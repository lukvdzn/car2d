#pragma once
#include <SFML/Graphics.hpp>

class World
{
public:
	World();
	void load_track(const char*);
	void draw(sf::RenderWindow&);
	sf::VertexArray& get_track_outline();
	sf::VertexArray& get_track_inline();
	sf::VertexArray& get_track_checkpoints();
private:
	sf::VertexArray t_checkpoints;
	sf::VertexArray t_outline;
	sf::VertexArray t_inline;
};