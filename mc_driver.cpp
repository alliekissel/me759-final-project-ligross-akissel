#include <iostream>
#include <cmath>
#include <vector>
#include "rng.h"

#define _USE_MATH_DEFINES
const float A=1;
// function signatures

float distance2collision(float MFP, float x, float y, float z, float r, float u, float v, float w);
int determine_reaction(const float sig_s, const float sig_a); 
void sample_isotropic(float* u, float* v, float* w);
void energy_angle_transfer(float* E, float* u, float* v, float* w);

// TODO slightly concerned about multiple threads touching the same data/ race conditions
// possible that reduction will be useful
struct ESTIMATOR
{
    int n; // the number of particles thus farr
    // TODO_LG should these be single values or arrays
    // if single value worry about race conditions/false sharing
    // if array, process at the end, but number on contributions is stochastic, so size is?
    // perhaps a vector would be best, so we could append contributions
    float tally_mean;
    float tally_re;
};

// create one vector of all the tracks and just append in the looping section
// post process it with a reduce with multiple threads to do efficientlyl and avoid having
// many threads tyring to touch the same memory address

// this function is responsible for driving and timing the montecarlo simulation
// TODO_LG
int main(int argc, char* argv[]) {
    unsigned int num_histories = atoi(argv[1]); // number of simulations
    unsigned int threads = atoi(argv[2]); // number of threads

    for(unsigned int i=0; i < num_histories; i++){
        // sample birth particle for energy(TODO_LG perhaps monoenergetic)
        // sample birth direciton 
        bool alive = true;
        while(alive) {
            // sample distance to collision
            // compute distance to sphere
                // if we leave the geometry
                    // compute track length inside sphere, update tally
                    // alive = false; or maybe break;
                // if we stay in geomtery
                    // update tally with track length contribution
            // sample reaction (absorbtion or scattering)
                // if(absoption)
                    // alive = false; or maybe break;
                // else if(scatter)
                    // sample/update outgoing E and (u,v,w)
        }
    }


    return 0;
}


// responsible for sampling exponential distribution and
// determining if the particle remains in the geometry
float distance2collision(float MFP, float x, float y, float z, float r, float u, float v, float w){
    // determine from current position and direction and 
    // new distance whether particle is leaving geometry
    // i.e. compute new x,y,z from old and determine if x*x+y*y+z*z<r*r
    // if in geometry
    float xi = gen_rand_0_to_1();
    float d = -log(xi)/MFP;
    // use current position, d , and direction to determine if we exit the sphere

    if(0) { // we exit
        // determine length inside the sphere
    }
    return d;

}

// accepts non-normalized cross sections and samples to determine reaction type
// returns 0 for scatter and 1 for absortion
int determine_reaction(const float sig_s, const float sig_a){

    float norm = sig_s + sig_a;
    float xi = gen_rand_0_to_1();
    if(xi < sig_s/norm ) {
        // scattering event
        return 0;
    } else {
        return 1;
    }
}

void sample_isotropic(float* u, float* v, float* w) {
    // generate a random pair
    float xi = gen_rand_0_to_1();
    float eta = gen_rand_0_to_1();
    *w = 2*xi - 1;
    *u = cos(2*M_PI*eta) * sqrt(  1 - (*w) * (*w)  );
    *v = sin(2*M_PI*eta) * sqrt(  1 - (*w) * (*w)  );
}

// determines the outgoing energy and angle of an isotropic, elastic collision
// assumes mass number is defined above
void energy_angle_transfer(float* E, float* u, float* v, float* w) {
    // modify E,u,v,w in a safe way that gives new direction
    // (u,v,w) is incoming direciton
    // (U,V,W) is outgoing direction
    float E_in = *E; // store originaal energy 
    // generate a new isotropic direction
    float *U;
    float *V;
    float *W;
    sample_isotropic(U,V,W);
    // compute the dot product between incoming and exiting directions
    float mu = (*u) * (*U) + (*w) * (*W) + (*v) * (*V);
    // elastic scattering physics
    float factor = pow((1/(1+A)),2)*(1+A*A + 2*A*mu) ; 
    *E = E_in * factor;
}


