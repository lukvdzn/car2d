#pragma once
#include <SFML/Graphics.hpp>

class LevelEditor
{
public:
	LevelEditor();
	void draw(sf::RenderWindow&);
	bool done_with_edit() const;
	void save_to_file(const char*) const;

	// TODO, move to file serialisation
	sf::VertexArray track_outline, track_inline;


private:
	bool finished_track_outline, finished_both;

	void update(sf::RenderWindow&, sf::VertexArray&);
};
