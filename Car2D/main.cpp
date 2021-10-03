#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Mouse.hpp>
#include "Car.h"
#include "LevelEditor.h"
#include "GameManager.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1080, 720), "Car2D");
    window.setVerticalSyncEnabled(true);

    //LevelEditor editor;
    GameManager gm;
    gm.init("test_map");
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        /*if (editor.done_with_edit() && !done)
        {
            done = true;
            editor.save_to_file("test_map");
            gm.open_map("test_map");
        }
        else {
            editor.draw(window);
        }*/
        gm.draw(window);
        window.display();
    }

    return 0;
}