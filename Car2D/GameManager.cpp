#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Window/Keyboard.hpp>
#include "GameManager.h"
#include "VectorMath.h"

GameManager::GameManager() : delta_time{0}
{

}

void GameManager::init(const char* track_path)
{
	world.load_track(track_path);
}

void GameManager::draw(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) car.drive();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) car.turn(true);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) car.turn(false);
	world.draw(window);
	car.update_draw(window, world, delta_time);
	delta_time = clock.restart().asMilliseconds();
}