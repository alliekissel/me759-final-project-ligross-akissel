#include "rng.h"

unsigned int seed = 8675309;
std::mt19937 generator(seed);
const float min = 0.0, max = std::nextafter(1.0, std::numeric_limits<float>::max());
std::uniform_real_distribution<float> dist(min, max);

// generates a number between 0 and 1 with a high quality random number generator
float gen_rand_0_to_1() {
    float random_num = dist(generator);
    return random_num;
}