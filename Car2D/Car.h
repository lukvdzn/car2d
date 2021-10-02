#pragma once
#include <SFML/Graphics.hpp>

constexpr float car_width = 30;
constexpr float car_height = 50;
constexpr float c_axle_dist = 3.1f;
constexpr float c_drag = 0.4557f; // 0.4257f
constexpr float c_roll_resistance = 30.f * c_drag;
constexpr float c_max_engine_force = 3.f;

class Car
{
public:
	Car();
	void update_draw(sf::RenderWindow& window, sf::VertexArray& track_outline, sf::VertexArray& track_inline, float dt);

private:
	sf::Vector2f v_dir; // unit direction vector
	sf::Vector2f v_acc, v_vel, v_pos;
	float engine_force, mass, angle;

	sf::Texture texture;
	sf::Sprite car;

	sf::Vector2f ray_wall_intersect(sf::Vector2f&, sf::VertexArray&, sf::VertexArray&);
	bool intersect(const sf::Vector2f&, sf::VertexArray&, sf::VertexArray&);
};