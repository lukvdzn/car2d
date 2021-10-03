#include <random>
#include "AI.h"

AI::AI()
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
}

void AI::process_input(float input[3])
{
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
		out[i] = out[i] / (1 + std::abs(out[i]));
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

void AI::draw(sf::RenderWindow& window)
{
	//car.update_draw(window, dt);
}