#include <iostream>
#include <cmath>
#include <vector>
#include "rng.h"

// function signatures
float distance2collision(float MFP, float x, float y, float z);
int determine_reaction(const float sig_s, const float sig_a); // TODO_LG possible just to make this a global variable
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
float distance2collision(float MFP, float x, float y, float z){
    // call sample exponential
    // determine from current position and direction and 
    // new distance whether particle is leaving geometry
    // i.e. compute new x,y,z from old and determine if x*x+y*y+z*z<r*r
    // if in geometry
    float d = sample_exponential(MFP);
    // else
        // determine length before leaving sphere
        // d = distance to sphere
    return d;

}

// TODO_LG possible just to make this a global variable
int determine_reaction(const float sig_s, const float sig_a){
    // determine if we have scattering or absorption
    // if scatter
    return 0;
    // if absopriton
    //return 1;
}


// look into cookbook for isotropic scattering and the energy angle relation
// TODO_LG_AND_AK look at derivation for own personal curiousity
void energy_angle_transfer(float* E, float* u, float* v, float* w) {
    // modify E,u,v,w in a safe way that gives new direction
}


