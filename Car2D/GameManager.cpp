#include <iostream>
#include <fstream>
#include <cmath>
#include "GameManager.h"
#include "VectorMath.h"
GameManager::GameManager()
	: delta_time{ 0 },
	track_outline{ sf::LinesStrip },
	track_inline{ sf::LinesStrip }
{

}

void GameManager::draw(sf::RenderWindow& window)
{
	window.draw(track_outline);
	window.draw(track_inline);
	car.update_draw(window, track_outline, track_inline, delta_time);
	delta_time = clock.restart().asMilliseconds();
}

/*
sf::Vector2f GameManager::ray_wall_intersect(const sf::Vector2f& v_pos, const sf::Vector2f& ray_dir)
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
}*/

void GameManager::open_map(const char* file_name)
{
	std::ifstream f_map{ file_name };
	if (!f_map)
	{
		std::cout << "Could not read file " << file_name << std::endl;
		return;
	}
	int size_track = -1;
	f_map >> size_track;
	for (auto i = 0; i < size_track; ++i)
	{
		float x, y;
		f_map >> x >> y;
		track_outline.append(sf::Vertex{ sf::Vector2f{ x, y } });
	}
	f_map >> size_track;
	for (auto i = 0; i < size_track; ++i)
	{
		float x, y;
		f_map >> x >> y;
		track_inline.append(sf::Vertex{ sf::Vector2f{ x, y } });
	}
	std::cout << "Done reading " << file_name << std::endl;
}