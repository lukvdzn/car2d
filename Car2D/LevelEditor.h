#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"

class LevelEditor : public GameState
{
public:
	LevelEditor();
	void draw(sf::RenderWindow&) override;
	bool done_with_edit() const;
	void save_to_file(const char*) const;

	// TODO, move to file serialisation
	sf::VertexArray track_outline, track_inline;


private:
	bool finished_track_outline, finished_both;

	void update(sf::RenderWindow&, sf::VertexArray&);
};
