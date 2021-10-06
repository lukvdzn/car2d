#include "PseudoRandom.h"

// initialise prng engine
std::default_random_engine PseudoRandom::eng{ std::random_device{ }() };

PseudoRandom::PseudoRandom()
{

}

PseudoRandom& PseudoRandom::get_instance()
{
	static PseudoRandom pr;
	return pr;
}

float PseudoRandom::real_random_range(float a, float b) const
{
	std::uniform_real_distribution<> dis(a, b);
	return dis(eng);
}

int PseudoRandom::int_random_range(int a, int b) const
{
	std::uniform_int_distribution<> dis(a, b);
	return dis(eng);
}