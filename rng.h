#ifndef RNG
#define RNG

#include <random>
#include <chrono>

std::random_device entropy_source;
std::mt19937 generator(entropy_source());
const float min = 0.0, max = std::nextafter(1.0, std::numeric_limits<float>::max());
std::uniform_real_distribution<float> dist(min, max);

// signatures
float gen_rand_0_to_1();

#endif