#pragma once
#include <SFML/Graphics.hpp>
#include "World.h"

constexpr float car_width = 15;
constexpr float car_height = 25;
constexpr float mass = 1000.f;
constexpr float c_inc_engine_force = 0.5f;
constexpr float c_max_rot = 20.f;
constexpr float c_inc_rot = 3.f;
constexpr float c_axle_dist = 1.f; // 3.1f
constexpr float c_drag = 0.4557f; // 0.4257f
constexpr float c_roll_resistance = 30.f * c_drag;
constexpr float c_max_engine_force = 2.f;
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
	sf::Vector2f ray_wall_intersect(sf::Vector2f&, World&, float&);
	bool on_track(const sf::Vector2f&, World&);

	sf::Vector2f v_dir; // unit direction vector
	sf::Vector2f v_acc;
	sf::Vector2f v_vel;
	sf::Vector2f v_pos;
	float engine_force;
	float angle;
	float time_between_checkpoints;
	bool turn_left;
	bool turn_right;
	bool drive_forward;
	int checkpoints_passed;
	sf::Sprite car;
};