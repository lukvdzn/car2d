#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"

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
	void set_texture(const sf::Texture&);
	void turn(bool left);
	void drive();
	void update_draw(sf::RenderWindow& window, World& world, float dt);

	float dist_left, dist_forward, dist_right, fitness;
	bool finished;

private:
	sf::Vector2f v_dir; // unit direction vector
	sf::Vector2f v_acc, v_vel, v_pos;
	float engine_force, mass, angle, time_between_checkpoints;
	bool turn_left, turn_right, drive_forward;
	int checkpoints_passed;
	sf::Sprite car;

	sf::Vector2f ray_wall_intersect(sf::Vector2f&, sf::VertexArray&, sf::VertexArray&, float&);
	bool intersect(const sf::Vector2f&, sf::VertexArray&, sf::VertexArray&);
};