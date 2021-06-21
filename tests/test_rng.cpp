#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

// random number generator using rand()
void gen_rand_0_to_1_test1(float* random_sequence, int n);
// random number generator using a distribution with time as seed
void gen_rand_0_to_1_test2(float* random_sequence, int n);
// random number generator using a distribution with entropy source as seed
void gen_rand_0_to_1_test3(float* random_sequence, int n);

int main(int argc, char *argv[])
{
    int n = 10000;
    int num_trials = 1;
    // create file to write output to
    std::ofstream file;
    file.open("random_sequences.txt");

    // test1
    float random_sequence1[n];
    gen_rand_0_to_1_test1(random_sequence1, n);

    for (int j = 0; j < num_trials; j++) {
        for (int i = 0; i < n; i++)
        {
            // write sequence to file
            file << "rs1_" << std::to_string(j) << " " << random_sequence1[i] << "\n";
        }
    }

    // test2
    float random_sequence2[n];
    gen_rand_0_to_1_test2(random_sequence2, n);

    for (int j = 0; j < num_trials; j++) {
        for (int i = 0; i < n; i++)
        {
            // write sequence to file
            file << "rs2_" << std::to_string(j) << " " << random_sequence2[i] << "\n";
        }
    }

    // test3
    float random_sequence3[n];
    gen_rand_0_to_1_test3(random_sequence3, n);

    for (int j = 0; j < num_trials; j++) {
        for (int i = 0; i < n; i++)
        {
            // write sequence to file
            file << "rs3_" << std::to_string(j) << " " << random_sequence3[i] << "\n";
        }
    }
    
    file.close();

    return 0;
}

void gen_rand_0_to_1_test1(float* random_sequence, int n) {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    srand(seed); // make this a different seed so that it can change for each test sequence
    for(int i = 0; i < n; i++) {
        random_sequence[i] = (float) rand() / (float) RAND_MAX;
    }
}

void gen_rand_0_to_1_test2(float* random_sequence, int n) {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    const float min = 0.0, max = std::nextafter(1.0, std::numeric_limits<float>::max());
    std::uniform_real_distribution<float> dist(min, max);
    for(int i = 0; i < n; i++) {
        random_sequence[i] = dist(generator);
    }
}

void gen_rand_0_to_1_test3(float* random_sequence, int n) {
    std::random_device entropy_source;
    std::mt19937 generator(entropy_source());
    const float min = 0.0, max = std::nextafter(1.0, std::numeric_limits<float>::max());
    std::uniform_real_distribution<float> dist(min, max);
    for(int i = 0; i < n; i++) {
        random_sequence[i] = dist(generator);
    }
}
