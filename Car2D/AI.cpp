#include <random>
#include "AI.h"

AI::AI(const sf::Texture& texture)
{
	std::random_device rd;
	std::default_random_engine eng{ rd() };
	std::uniform_real_distribution<> dis(-sqrt_hidden, sqrt_hidden);
	for (auto i = 0; i < 3; ++i)
	{
		for (auto j = 0; j < 3; ++j)
		{
			i_weights[i][j] = dis(eng);
			h_weights[i][j] = dis(eng);
		}
	}
	car.set_texture(texture);
}

void AI::process_input()
{
	float input[3] = { car.dist_forward, car.dist_left, car.dist_right };
	float temp[3] = { 0, 0, 0 };
	float out[3] = { 0, 0, 0 };
	for (auto i = 0; i < 3; ++i)
	{
		for (auto j = 0; j < 3; ++j)
		{
			temp[i] += i_weights[i][j] * input[j];
		}
		// fast sigmoid function
		temp[i] = temp[i] / (1 + std::abs(temp[i]));
	}
	for (auto i = 0; i < 3; ++i)
	{
		for (auto j = 0; j < 3; ++j)
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
	for (auto i = 0; i < 3; ++i)
	{
		for (auto j = 0; j < 3; ++j)
		{
			baby.i_weights[i][j] = dis(eng) ? i_weights[i][j] : other.i_weights[i][j];
			baby.h_weights[i][j] = dis(eng) ? h_weights[i][j] : other.h_weights[i][j];
		}
	}

	return baby;
}

void AI::mutate()
{
	std::random_device rd;
	std::default_random_engine eng{ rd() };
	std::uniform_int_distribution<> idis(0, 100);
	std::uniform_real_distribution<> rdis(-sqrt_hidden, sqrt_hidden);
	
	for (auto i = 0; i < 3; ++i)
	{
		for (auto j = 0; j < 3; ++j)
		{
			if (idis(eng) < 5)
			{
				i_weights[i][j] = rdis(eng);
			}
			if (idis(eng) < 5)
			{
				h_weights[i][j] = rdis(eng);
			}
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