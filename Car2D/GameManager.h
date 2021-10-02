#pragma once
#include <SFML/Graphics.hpp>
#include "LevelEditor.h"
#include "Car.h"

class GameManager
{
public:
	GameManager();

	void open_map(const char* file_name);
	void draw(sf::RenderWindow&);
	//sf::Vector2f ray_wall_intersect(const sf::Vector2f&, const sf::Vector2f&);

private:
	float delta_time;
	sf::Clock clock;
	sf::VertexArray track_outline, track_inline;
	Car car;
};
