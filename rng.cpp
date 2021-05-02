#include <random>
#include <chrono>
#include "rng.h"

// generates a number between 0 and 1 with a high quality random number generator
float gen_rand_0_to_1() {
    std::random_device entropy_source;
    std::mt19937 generator(entropy_source());
    const float min = 0.0, max = std::nextafter(1.0, std::numeric_limits<float>::max());
    std::uniform_real_distribution<float> dist(min, max);
    float random_num = dist(generator);
    return random_num;
}

// sample a continuous exponential distribution
float sample_exponential(float mfp){
    return mfp;
}

