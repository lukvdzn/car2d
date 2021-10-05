#pragma once
#include <SFML/Graphics.hpp>

/*
	First draws track outline, then inner trackline and last the checkpoints for fitness computation.
	Checkpoints should be drawn from outline to track inline, as for checkpoints pass evaluation front
	tires are checked, if one of them is on the left side of the checkpoint line.
*/

class LevelEditor
{
public:
	LevelEditor();
	void draw(sf::RenderWindow&);
	bool done_with_edit() const;
	void save_to_file(const char*) const;

private:
	sf::VertexArray track_outline;
	sf::VertexArray track_inline;
	sf::VertexArray track_checkpoints;
	bool mouse_pressed;
	bool done_outline;
	bool done_inline;
	bool done_checkpoints;

	void update(sf::RenderWindow&);
};
