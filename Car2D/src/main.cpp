#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Car.h"
#include "LevelEditor.h"
#include "GameManager.h"
#include "GameConstants.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(GameConstants::window_width, GameConstants::window_height), 
		"Car2D");
    window.setVerticalSyncEnabled(true);

    //LevelEditor editor;
    GameManager gm;
    gm.init("track_1");
    
    while (window.isOpen())
    {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
        window.clear();
		gm.draw(window);
        window.display();
    }

    return 0;
}