#pragma once
#include <SFML/Graphics.hpp>
#include "Car.h"

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
	AI();
	void process_input(float input[3]);
	void draw(sf::RenderWindow&);

private:
	Car car;
	float i_weights[3][3];
	float h_weights[3][3];
};