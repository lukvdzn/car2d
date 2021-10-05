#include "AI.h"
#include "PseudoRandom.h"

AI::AI(const sf::Texture& texture) 
	: n_weights(total_weights), 
	car{ texture }
{
	const PseudoRandom& pr = PseudoRandom::get_instance();
	for (auto i = 0; i < total_weights; ++i)
	{
		n_weights[i] = pr.real_random_range(-sqrt_hidden, sqrt_hidden);
	}
}

void AI::process_input()
{
	// input is car.dist_to_walls
	float temp[5] = { 0 };
	float out[3] = { 0 };
	auto l_fast_sigmoid = [](float& x) {
		x = x / (1 + std::abs(x));
	};
	constexpr int hidden_to_out_index = hidden_nodes * input_nodes;
	for (auto i = 0; i < hidden_nodes; ++i)
	{
		for (auto j = 0; j < input_nodes; ++j)
		{
			temp[i] += n_weights[i * hidden_nodes + j] * car.dist_to_walls[j];
		}
		l_fast_sigmoid(temp[i]);
	}
	for (auto i = 0; i < output_nodes; ++i)
	{
		for (auto j = 0; j < hidden_nodes; ++j)
		{
			out[i] += n_weights[hidden_to_out_index +  i * output_nodes + j] * temp[j];
		}
		l_fast_sigmoid(out[i]);
		out[i] = std::abs(out[i]);
	}
	if (out[0] > output_threshold) car.drive();
	if (out[1] > out[2])
	{
		// left
		if (out[1] > output_threshold) car.turn(true);
	}
	else {
		// right
		if (out[2] > output_threshold) car.turn(false);
	}
}

AI AI::cross_over(const AI& other, const sf::Texture& texture) const
{
	const PseudoRandom& pr = PseudoRandom::get_instance();
	AI baby{ texture };
	for (auto i = 0; i < total_weights; ++i)
	{
		baby.n_weights[i] = pr.int_random_range(0, 1) ? n_weights[i] : other.n_weights[i];
	}
	return baby;
}

void AI::mutate()
{
	// mutation rate ~ 0.02
	const PseudoRandom& pr = PseudoRandom::get_instance();
	if(pr.int_random_range(0, 100) <= 2)
	{
		// index of genome to change, genomes are made up to 1D vector from all weights in network appended
		int i = pr.int_random_range(0, total_weights - 1);
		n_weights[i] = pr.real_random_range(-sqrt_hidden, sqrt_hidden);
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