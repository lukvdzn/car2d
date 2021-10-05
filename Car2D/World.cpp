#include <iostream>
#include <fstream>
#include "World.h"

World::World() 
	: t_checkpoints{ sf::Lines }, 
	t_outline{ sf::LinesStrip }, 
	t_inline{ sf::LinesStrip }
{

}

void World::draw(sf::RenderWindow& window)
{
	window.draw(t_outline);
	window.draw(t_inline);
	// window.draw(t_checkpoints);
}

void World::load_track(const char* file_name)
{
	std::ifstream f_map{ file_name };
	if (!f_map)
	{
		std::cout << "Could not read file " << file_name << std::endl;
		return;
	}
	auto l_append = [&](sf::VertexArray& va) {
		int size_points = -1;
		f_map >> size_points;
		for (auto i = 0; i < size_points; ++i)
		{
			float x, y;
			f_map >> x >> y;
			va.append(sf::Vertex{ sf::Vector2f{ x, y } });
		}
	};
	l_append(t_outline);
	l_append(t_inline);
	l_append(t_checkpoints);
	std::cout << "Done reading track: " << file_name << std::endl;
}

sf::VertexArray& World::get_track_outline()
{
	return t_outline;
}

sf::VertexArray& World::get_track_inline()
{
	return t_inline;
}

sf::VertexArray& World::get_track_checkpoints()
{
	return t_checkpoints;
}