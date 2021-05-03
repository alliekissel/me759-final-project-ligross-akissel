#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include "rng.h"

const float A=1;
// function signatures

float distance2collision(float MFP, float *x, float *y, float *z, float r, const float u, const float v, const float w, bool *termination);
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

    // test RNG consistency between sequences with same seed
    float* random_sequence = new float[20];
    for(int i = 0; i < 20; i++) {
        random_sequence[i] = gen_rand_0_to_1();
        std::cout << random_sequence[i] << std::endl;
    }

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
            alive = false;
        }
    }
    return 0;
}


// responsible for sampling exponential distribution and
// determining if the particle remains in the geometry
float distance2collision(float MFP, float *x, float *y, float *z, float r, const float u, const float v, const float w, bool *termination) {
    // sample distance to event from exponential distribution
    float xi = gen_rand_0_to_1();
    float d = -log(xi)/MFP;
    // find if final position is inside or outside of sphere
    float mag_A_sq = (*x)*(*x)+(*y)*(*y)+(*z)*(*z);
    float mag_A = sqrt(mag_A_sq);
    // particle travels distance d along direction (u,v,w) from starting point (x,y,z)
    // the magnitude of the vector for the point the particle is transported to
    float mag_end_sq = (*x+d*u)*(*x+d*u)+(*y+d*v)*(*y+d*v)+(*z+d*w)*(*z+d*w);
    // determine if the hitsory is terminated by exiting the geometry or if the history will continue
    if(mag_end_sq > r*r) { 
        // we exit the geometry
        // determine length inside the sphere
        if(mag_A==0){
            // we are at the origin and the next collision would happen outside the sphere
            // the track length is just the radius
            // must do this becasue there will be a divide by zero to compute the costheta otherwise
            *termination = true;
            return r;
        } else {
            // if we are not at the origin, but leave the sphere, determine the part of the track
            // inside the geometry, named d_i
            float costheta = -(mag_end_sq - mag_A_sq - d*d)/(2*mag_A*d); 
            // determine d_i from quadratic
            float a,b,c;
            a = 1.0f;
            b = -2*mag_A*costheta;
            c = mag_A_sq - r*r;
            // TOOD_LG try and justify that we will always take the plus root, if we can't show this, then just take
            // which ever is positive between the two below. pray that there is never a case where they are both positive
            float d_i_plus = (-b + sqrt(b*b- 4*a*c))/(2*a);
            float d_i_minus = (-b - sqrt(b*b - 4*a*c))/(2*a);
            float d_i  = (d_i_plus >= 0) ? d_i_plus : d_i_minus; // if d_i plus is zero, 4*a*c=0 and thus d_i_minus = -2b and should not be returned
            *termination = true;
            return d_i;
        }
    } else {
        // position transported to remains inside sphere all of track contributes
        // update positions continue on to the next history
        *x += d*u;
        *y += d*v;
        *z += d*w;
        *termination = false;
        return d;
    }
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


