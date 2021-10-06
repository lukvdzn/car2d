#include <iostream>
#include <algorithm>
#include <utility>
#include <fstream>
#include <cmath>
#include <random>
#include <SFML/Window/Keyboard.hpp>
#include "GameManager.h"
#include "PseudoRandom.h"
#include "VectorMath.h"

GameManager::GameManager() 
    : delta_time{ 0 }, 
    reset{ false }
{

}

void GameManager::init(const char* track_path)
{
    car_texture.loadFromFile(car_file_path);
	world.load_track(track_path);
    for (auto i = 0; i < population_size; ++i) population.emplace_back(car_texture);
}

void GameManager::draw(sf::RenderWindow& window)
{
    reset = true;
	world.draw(window);
	std::for_each(population.begin(), population.end(), [&](AI& ai) {
		ai.draw(window, world, delta_time); 
		ai.process_input();
        if (reset && !ai.has_collided()) reset = false;
	});
    delta_time = clock.restart().asMilliseconds();
    if (reset) roulette_wheel_selection();
}

void GameManager::roulette_wheel_selection()
{
	PseudoRandom& pr = PseudoRandom::get_instance();
    std::vector<AI> new_generation;
    float sum_fitness = 0.f;
    std::for_each(population.begin(), population.end(), [&](AI& ai) { sum_fitness += ai.get_fitness(); });
    
    // Cross Over
    while (new_generation.size() < population_size)
    {
        auto l_turn_wheel = [&]() {
            float rand = pr.real_random_range(0, sum_fitness);
            float wheel_sum = 0.f;
            int j = 0;
            for (; wheel_sum < rand && j < population_size; ++j)
            {
                wheel_sum += population[j].get_fitness();
            }
            return j == 0 ? 0 : j - 1;
        };
        auto& parent1 = population[l_turn_wheel()];
        auto& parent2 = population[l_turn_wheel()];
        new_generation.push_back(parent1.cross_over(parent2, car_texture));
    }
    population = std::move(new_generation);
    // Mutation
    std::for_each(population.begin(), population.end(), [&](AI& ai) { ai.mutate(); });
    std::cout << "Average fitness: " << sum_fitness / population_size << std::endl;
}