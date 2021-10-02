#pragma once
#include <SFML/Graphics.hpp>

class GameState
{
public:
	virtual void draw(sf::RenderWindow&) = 0;

protected:
	bool finished;
};

