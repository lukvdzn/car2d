#include <iostream>
#include <SFML/Graphics.hpp>s
#include <algorithm>
#include "Car.h"
#include "VectorMath.h"

Car::Car() 
	: v_dir{0, -1}, 
	v_acc{0, 0}, 
	v_vel{0, 0}, 
	v_pos{50, 480}, 
	engine_force{0}, 
	mass{1000.f}, 
	angle {0}, 
	turn_left{false}, 
	turn_right{false}, 
	drive_forward{false},
	checkpoints_passed{0}
{
	if (!texture.loadFromFile("top_down_yellow.png"))
	{
		std::cout << "Something went wrong opening sprite file" << std::endl;
	}
	car.setTexture(texture);
}

sf::Vector2f Car::ray_wall_intersect(sf::Vector2f& ray_dir,  sf::VertexArray& track_outline, sf::VertexArray& track_inline)
{
	float dist_nearest = 1000000000.f;
	sf::Vector2f v_nearest = { -1, -1 };
	// Find nearest collision point for each wall on each track
	for (auto& track : { track_inline, track_outline })
	{
		for (auto i = 1; i < track.getVertexCount(); ++i)
		{
			// https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
			const auto& p_a = track[i - 1].position;
			const auto& p_b = track[i].position;
			auto v_1 = v_pos - p_a;
			auto v_2 = p_b - p_a;
			auto v_3 = sf::Vector2f{ -ray_dir.y, ray_dir.x };
			auto vd = v_dot(v_2, v_3);

			if (std::abs(vd) > 0.000001f)
			{
				// cross product by 2d determinant
				float t_1 = (v_2.x * v_1.y - v_2.y * v_1.x) / vd;
				float t_2 = v_dot(v_1, v_3) / vd;
				if (t_1 >= 0.0 && (t_2 >= 0.f && t_2 <= 1.0f))
				{
					if (t_1 < dist_nearest)
					{
						dist_nearest = t_1;
						v_nearest = v_pos + t_1 * ray_dir;
					}
				}
			}
		}
	}
	return v_nearest;
}

void Car::update()
{

}

void Car::update_draw(sf::RenderWindow& window, World& world, float dt)
{
	if (drive_forward)
	{
		engine_force = std::min(c_max_engine_force, engine_force + 0.5f);
	}
	else {
		engine_force = std::max(0.f, engine_force - 0.5f);
	}

	if (turn_left)
	{
		angle = std::max(-20.f, angle - 3.f);
	}
	else if (turn_right)
	{
		angle = std::min(20.f, angle + 3.f);
	}

	float speed = v_magnitude(v_vel);
	sf::Vector2f v_trac_force = v_dir * engine_force;
	sf::Vector2f v_drag_force = -c_drag * v_vel * speed;
	sf::Vector2f v_roll_res_force = -c_roll_resistance * v_vel;
	sf::Vector2f v_long_force = v_trac_force + v_drag_force + v_roll_res_force;

	//should be net force, not long_force
	v_acc = v_long_force / mass;

	//euler integration method
	v_vel = v_vel + dt * v_acc;
	v_pos = v_pos + dt * v_vel;

	float turning_radius = c_axle_dist / std::sin(deg_to_rad(angle));
	float angular_velocity = speed / turning_radius;
	v_rotate(v_dir, angular_velocity);

	//first set origin to car midpoint, rotateand then translate to position
	car.setOrigin(car_width / 2.f, car_height / 2.f);
	car.rotate(rad_to_deg(angular_velocity));
	car.setPosition(v_pos);
	
	// ---------------------------------- Rays ------------------------------------------------------------------------
	sf::Vector2f ray_left = {-v_dir.y, v_dir.x};
	sf::Vector2f ray_right = { v_dir.y, -v_dir.x };

	sf::Vector2f lray_is = ray_wall_intersect(ray_left, world.get_track_outline(), world.get_track_inline());
	sf::Vector2f rray_is = ray_wall_intersect(ray_right, world.get_track_outline(), world.get_track_inline());
	sf::Vector2f sray_is = ray_wall_intersect(v_dir, world.get_track_outline(), world.get_track_inline());

	const auto& trf = car.getTransform();
	sf::Vector2f tire_left_front = trf.transformPoint({ 0, 0 });
	sf::Vector2f tire_left_back = trf.transformPoint({ 0, car_height });
	sf::Vector2f tire_right_front = trf.transformPoint({ car_width, 0 });
	sf::Vector2f tire_right_back = trf.transformPoint({ car_width, car_height });

	// ---------------------------------- Checkpoints -----------------------------------------------------------------
	auto& checkpoints = world.get_track_checkpoints();
	// might have passed multiple laps -> checkpoints_passed could be out of bounds
	auto& vertex1 = checkpoints[checkpoints_passed % checkpoints.getVertexCount()];
	auto& vertex2 = checkpoints[(checkpoints_passed + 1) % checkpoints.getVertexCount()];
	const auto& v1pos = mirror(vertex1.position);
	const auto& v2pos = mirror(vertex2.position);
	// if one of the 2 front tires passes next checkpoint, update color
	if (ccw(v1pos, v2pos, mirror(tire_left_front)) >= 0.f || ccw(v1pos, v2pos, mirror(tire_right_front)) >= 0.f)
	{
		checkpoints_passed += 2;
		vertex1.color = sf::Color::Green;
		vertex2.color = sf::Color::Green;
	}

	// ---------------------------------- Collision -------------------------------------------------------------------
	for (auto& p : { tire_left_front, tire_left_back, tire_right_front, tire_right_back })
	{
		if (intersect(p, world.get_track_outline(), world.get_track_inline()))
		{
			// Calculate fitness by checking time and how many checkpoints car has passed
			break;
		}
	}

	// ----------------------------------- Drawing --------------------------------------------------------------------
	sf::Vertex ls[] = { sf::Vertex{v_pos}, sf::Vertex{sray_is} };
	sf::Vertex ll[] = { sf::Vertex{v_pos}, sf::Vertex{lray_is} };
	sf::Vertex lr[] = { sf::Vertex{v_pos}, sf::Vertex{rray_is} };
	ls[0].color = sf::Color::Yellow;
	ls[1].color = sf::Color::Yellow;
	ll[0].color = sf::Color::Red;
	ll[1].color = sf::Color::Red;
	lr[0].color = sf::Color::Green;
	lr[1].color = sf::Color::Green;
	window.draw(ls, 2, sf::Lines);
	window.draw(ll, 2, sf::Lines);
	window.draw(lr, 2, sf::Lines);
	window.draw(car);

	// ---------------------------------- Reset --------------------------------------------------------------------------
	turn_left = false;
	turn_right = false;
	drive_forward = false;
}

// Only works, if map track outlines form a convex polygon
bool Car::intersect(const sf::Vector2f& p, sf::VertexArray& track_outline, sf::VertexArray& track_inline)
{
	// has to be to the right of track_outline
	for (auto i = 1; i < track_outline.getVertexCount(); ++i)
	{
		const auto& q = track_outline[i - 1].position;
		const auto& v = track_outline[i].position;
		if (ccw(q, v, p) <= 0.f)
		{
			return true;
		}
	}
	bool inside = true;
	// has to be to the left of track inline
	for (auto i = 1; i < track_inline.getVertexCount(); ++i)
	{
		const auto& q = track_inline[i - 1].position;
		const auto& v = track_inline[i].position;
		if (ccw(q, v, p) <= 0.f)
		{
			inside = false;
		}
	}

	return inside;
}

void Car::turn(bool left)
{
	if (left) turn_left = true;
	else turn_right = true;
}

void Car::drive()
{
	drive_forward = true;
}