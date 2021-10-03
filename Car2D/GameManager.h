#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Car.h"

class GameManager
{
public:
	GameManager();
	void init(const char* track_path);
	void draw(sf::RenderWindow&);

private:
	World world;
	Car car;
	float delta_time;
	sf::Clock clock;
};