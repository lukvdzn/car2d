#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"

constexpr float car_width = 15;
constexpr float car_height = 25;
constexpr float c_axle_dist = 3.1f;
constexpr float c_drag = 0.4557f; // 0.4257f
constexpr float c_roll_resistance = 30.f * c_drag;
constexpr float c_max_engine_force = 3.f;

// cos(45 deg), sin(45 deg)
constexpr float c_cs_45 = 0.70710678118f;

class Car
{
public:
	Car(const sf::Texture&);
	void turn(bool left);
	void drive();
	void update_draw(sf::RenderWindow&, World&, float dt);

	float dist_to_walls[5];
	float fitness;
	bool finished;

private:
	sf::Vector2f v_dir; // unit direction vector
	sf::Vector2f v_acc, v_vel, v_pos;
	float engine_force, mass, angle, time_between_checkpoints;
	bool turn_left, turn_right, drive_forward;
	int checkpoints_passed;
	sf::Sprite car;

	sf::Vector2f ray_wall_intersect(sf::Vector2f&, World&, float&);
	bool on_track(const sf::Vector2f&, World&);
};