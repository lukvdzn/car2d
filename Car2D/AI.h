#pragma once
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "World.h"

constexpr int input_nodes = 5;
constexpr int hidden_nodes = 5;
constexpr int output_nodes = 3;
constexpr int total_weights = input_nodes * hidden_nodes + hidden_nodes * output_nodes;
constexpr float sqrt_hidden = 2.2360679774997f;

enum class Decision {
	FORWARD = 0x1,
	FORWARD_LEFT = 0x11,
	FORWARD_RIGHT = 0x101,
	LEFT = 0x10,
	RIGHT = 0x100
};

class AI
{
public:
	AI(const sf::Texture&);
	AI cross_over(AI& other, const sf::Texture&);
	void mutate();
	void process_input();
	void draw(sf::RenderWindow&, World&, float);
	float get_fitness();
	bool has_collided();

private:
	AI();
	Car car;
	float i_weights[hidden_nodes][input_nodes];
	float h_weights[output_nodes][hidden_nodes];
};