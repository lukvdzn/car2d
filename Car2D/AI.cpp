#include <iostream>
#include <random>
#include "AI.h"

AI::AI(const sf::Texture& texture)
{
	std::random_device rd;
	std::default_random_engine eng{ rd() };
	std::uniform_real_distribution<> dis(-sqrt_hidden, sqrt_hidden);
	for (auto i = 0; i < hidden_nodes; ++i)
	{
		for (auto j = 0; j < input_nodes; ++j)
		{
			i_weights[i][j] = dis(eng);
		}
	}
	for (auto i = 0; i < output_nodes; ++i)
	{
		for (auto j = 0; j < hidden_nodes; ++j)
		{
			h_weights[i][j] = dis(eng);
		}
	}
	car.set_texture(texture);
}

void AI::process_input()
{
	// input is car.dist_to_walls
	float temp[5] = { 0, 0, 0 };
	float out[3] = { 0, 0, 0 };
	for (auto i = 0; i < hidden_nodes; ++i)
	{
		for (auto j = 0; j < input_nodes; ++j)
		{
			temp[i] += i_weights[i][j] * car.dist_to_walls[j];
		}
		// fast sigmoid function
		temp[i] = temp[i] / (1 + std::abs(temp[i]));
	}
	for (auto i = 0; i < output_nodes; ++i)
	{
		for (auto j = 0; j < hidden_nodes; ++j)
		{
			out[i] += h_weights[i][j] * temp[j];
		}
		// fast sigmoid function
		out[i] = std::abs(out[i]) / (1 + std::abs(out[i]));
	}

	if (out[0] >= 0.5f) car.drive();
	if (out[1] > out[2])
	{
		// left
		if (out[1] >= 0.5f) car.turn(true);
	}
	else {
		// right
		if (out[2] >= 0.5f) car.turn(false);
	}
}

AI AI::cross_over(AI& other, const sf::Texture& texture)
{
	std::random_device rd;
	std::default_random_engine eng{ rd() };
	std::uniform_int_distribution<> dis(0, 1);
	AI baby{ texture };
	for (auto i = 0; i < hidden_nodes; ++i)
	{
		for (auto j = 0; j < input_nodes; ++j)
		{
			baby.i_weights[i][j] = dis(eng) ? i_weights[i][j] : other.i_weights[i][j];
		}
	}
	for (auto i = 0; i < output_nodes; ++i)
	{
		for (auto j = 0; j < hidden_nodes; ++j)
		{
			baby.h_weights[i][j] = dis(eng) ? h_weights[i][j] : other.h_weights[i][j];
		}
	}

	return baby;
}

void AI::mutate()
{
	// mutation rate 0.02
	std::random_device rd;
	std::default_random_engine eng{ rd() };
	std::uniform_int_distribution<> idis(0, 100);
	std::uniform_real_distribution<> rdis(-sqrt_hidden, sqrt_hidden);

	if(idis(eng) <= 2)
	{
		int i = idis(eng) % total_weights;
		// weights from hidden nodes to output nodes
		if (i >= input_nodes * hidden_nodes)
		{
			i %= output_nodes * hidden_nodes;
			int j = i % hidden_nodes;
			i /= hidden_nodes;
			h_weights[i][j] = rdis(eng);
		}
		else {
			// weights from input nodes to hidden nodes
			i %= hidden_nodes * input_nodes;
			int j = i % input_nodes;
			i /= input_nodes;
			i_weights[i][j] = rdis(eng);
		}
	}
}

void AI::draw(sf::RenderWindow& window, World& world, float dt)
{
	car.update_draw(window, world, dt);
}

float AI::get_fitness()
{
	return car.fitness;
}

bool AI::has_collided()
{
	return car.finished;
}