#pragma once
#include <cmath>
#include <SFML/Graphics.hpp>

namespace Constants {
	constexpr float big_value = 1000000000.f;
};

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

inline bool on_lsegment(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c)
{
	if (b.x <= std::max(a.x, c.x) && b.x >= std::min(a.x, c.x) && b.y <= std::max(a.y, c.y) && b.y >= std::min(a.y, c.y))
		return true;
	return false;
}

// https://rootllama.wordpress.com/2014/06/20/ray-line-segment-intersection-test-in-2d/
inline float ray_line_intersection(const sf::Vector2f& p_a, const sf::Vector2f& p_b, 
	const sf::Vector2f& ray_orig, const sf::Vector2f& ray_dir)
{
	auto v_1 = ray_orig - p_a;
	auto v_2 = p_b - p_a;
	auto v_3 = sf::Vector2f{ -ray_dir.y, ray_dir.x };
	auto vd = v_dot(v_2, v_3);

	if (std::abs(vd) > 0.000001f)
	{
		// cross product by 2d determinant
		float t_1 = (v_2.x * v_1.y - v_2.y * v_1.x) / vd;
		float t_2 = v_dot(v_1, v_3) / vd;
		if (t_1 >= 0.0 && (t_2 >= 0.f && t_2 <= 1.0f))
			return t_1;
	}

	// big arbitrary value
	return Constants::big_value;
}