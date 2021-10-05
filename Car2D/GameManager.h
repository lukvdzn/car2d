#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Car.h"
#include "AI.h"

constexpr int population_size = 500;

class GameManager
{
public:
	GameManager();
	void init(const char* track_path);
	void draw(sf::RenderWindow&);
	void roulette_wheel_selection();

private:
	sf::Texture car_texture;
	World world;
	std::vector<AI> population;
	float delta_time;
	sf::Clock clock;
	bool reset;
};