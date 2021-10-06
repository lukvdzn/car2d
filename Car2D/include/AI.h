#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "World.h"

constexpr int input_nodes = 5;
constexpr int hidden_nodes = 5;
constexpr int output_nodes = 3;
constexpr int total_weights = input_nodes * hidden_nodes + hidden_nodes * output_nodes;
constexpr float c_bias = 1.f; 
constexpr float sqrt_hidden = 2.2360679774997f; // sqrt of |hidden_nodes|
constexpr float output_threshold = 0.5f;

/* Neural Network with 5 input nodes, 5 hidden nodes and 3 output for accelerating, steering left and steering right */
class AI
{
public:
	AI(const sf::Texture&);
	AI cross_over(const AI& other, const sf::Texture&) const;
	void mutate();
	void process_input();
	void draw(sf::RenderWindow&, World&, float);
	float get_fitness();
	bool has_collided();

private:
	AI();
	Car car;
	// network weights will be flattened 1D vector, where each weight is a genome of the AI
	std::vector<float> n_weights;
};