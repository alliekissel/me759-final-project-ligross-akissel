#include "rng.h"

// generates a number between 0 and 1 with a high quality random number generator
float gen_rand_0_to_1() {
    float random_num = dist(generator);
    return random_num;
}