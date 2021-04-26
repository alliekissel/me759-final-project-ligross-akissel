#include <iostream>
#include <random>
#include <chrono>

// random number generator using rand()
float gen_rand_0_to_1_test1();
// random number generator using a distribution with time as seed
float gen_rand_0_to_1_test2();
// random number generator using a distribution with entropy source as seed
float gen_rand_0_to_1_test3();
// plot collection of sequences seeded differently and plot on a 2D grid to 
// demonstrate quality of randomness
void plot_randomness();

float gen_rand_0_to_1_test1() {
    const float RANGE = 1.0f;
    float random_val = (float) rand() / (float) RANGE;
    return random_val;
}

float gen_rand_0_to_1_test2() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    const float min = 0.0, max = 1.0;
    std::uniform_real_distribution<float> dist(min, max);
    float random_val = dist(generator);
    return random_val;
}

float gen_rand_0_to_1_test3() {
    std::random_device entropy_source;
    std::mt19937 generator(entropy_source());
    const float min = 0.0, max = 1.0;
    std::uniform_real_distribution<float> dist(min, max);
    float random_val = dist(generator);
    return random_val;
}

void plot_randomness() {
    
}

int main(int argc, char* argv[]) {
    srand(2438298); // make this a different seed so that it can change for each test sequence
    
    // pass in loop number to each function so that you don't reseed for each sequence? 
    // rather than looping in main function

}
