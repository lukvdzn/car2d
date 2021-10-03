#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>

inline float v_magnitude(const sf::Vector2f& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

inline void v_rotate(sf::Vector2f& v, float rad_angle)
{
	float x_temp = v.x;
	v.x = v.x * std::cos(rad_angle) - v.y * std::sin(rad_angle);
	v.y = x_temp * std::sin(rad_angle) + v.y * std::cos(rad_angle);
}

inline float deg_to_rad(float deg)
{
	return deg * (M_PI / 180.f);
}

inline float rad_to_deg(float rad)
{
	return rad * (180.f / M_PI);
}

inline float v_dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

inline float ccw(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c)
{
	auto x = ((c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y));
	return x == 0.f ? 0.f : (x < 0.f ? -1.f : 1.f);
}

// Computer graphics/ SFML origin (0,0) is top left corner of window, some operations need it to be corrected
inline sf::Vector2f mirror(const sf::Vector2f& p)
{
	return { p.x, 720 - p.y };
}