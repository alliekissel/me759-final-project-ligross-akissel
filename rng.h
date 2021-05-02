#ifndef RNG
#define RNG

#include <random>

unsigned int seed = 8675309;
std::mt19937 generator(seed);
const float min = 0.0, max = std::nextafter(1.0, std::numeric_limits<float>::max());
std::uniform_real_distribution<float> dist(min, max);

// signatures
float gen_rand_0_to_1();

#endif