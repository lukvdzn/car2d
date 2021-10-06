#pragma once
#include <random>

class PseudoRandom
{
public:
	PseudoRandom(const PseudoRandom&) = delete;
	void operator=(const PseudoRandom&) = delete;
	static PseudoRandom& get_instance();
	float real_random_range(float, float) const;
	int int_random_range(int, int) const;

private:
	PseudoRandom();
	static std::default_random_engine eng;
};