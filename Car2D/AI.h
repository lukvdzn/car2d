#pragma once
#include <SFML/Graphics.hpp>
#include "Car.h"
#include "World.h"

constexpr float sqrt_hidden = 1.7320508f;

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
	float i_weights[3][3];
	float h_weights[3][3];
};