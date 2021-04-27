#include <iostream>
#include <random>
#include <chrono>

// random number generator using rand()
void gen_rand_0_to_1_test1(float* random_sequence, int n);
// random number generator using a distribution with time as seed
void gen_rand_0_to_1_test2(float* random_sequence, int n);
// random number generator using a distribution with entropy source as seed
void gen_rand_0_to_1_test3(float* random_sequence, int n);
// plot collection of sequences seeded differently and plot on a 2D grid to 
// demonstrate quality of randomness
void plot_randomness();

void gen_rand_0_to_1_test1(float* random_sequence, int n) {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed); // make this a different seed so that it can change for each test sequence
    for(int i = 0; i < n; i++) {
        random_sequence[i] = (float) rand() / (float) RAND_MAX;
    }
}

void gen_rand_0_to_1_test2(float* random_sequence, int n) {
    // @TODO add line to correct range from [0,1) to [0,1] (Piazza)
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    const float min = 0.0, max = 1.0;
    std::uniform_real_distribution<float> dist(min, max);
    for(int i = 0; i < n; i++) {
        random_sequence[i] = dist(generator);
    }
}

void gen_rand_0_to_1_test3(float* random_sequence, int n) {
    // @TODO add line to correct range from [0,1) to [0,1] (Piazza)
    std::random_device entropy_source;
    std::mt19937 generator(entropy_source());
    const float min = 0.0, max = 1.0;
    std::uniform_real_distribution<float> dist(min, max);
    for(int i = 0; i < n; i++) {
        random_sequence[i] = dist(generator);
    }
}

void plot_randomness() {
    // @TODO plot histogram to test uniformity of distribution
    // @TODO plot bitmap for a visual representation of randomness
    // @TODO possible also do a chi-square or Kolmogorov-Smirnov test for statistical analysis
    
}

int main(int argc, char *argv[])
{
    int n = 10;
    // test1
    float random_sequence1[n];
    gen_rand_0_to_1_test1(random_sequence1, n);

    std::cout << "Test 1 sequence" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << random_sequence1[i] << std::endl;
    }
    std::cout << std::endl;

    // test2
    float random_sequence2[n];
    gen_rand_0_to_1_test2(random_sequence2, n);

    std::cout << "Test 2 sequence" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << random_sequence2[i] << std::endl;
    }
    std::cout << std::endl;

    // test3
    float random_sequence3[n];
    gen_rand_0_to_1_test3(random_sequence3, n);

    std::cout << "Test 3 sequence" << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::cout << random_sequence3[i] << std::endl;
    }
    std::cout << std::endl;

    return 0;
}
