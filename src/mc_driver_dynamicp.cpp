#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <math.h>
#include <vector>
#include <chrono>
#include <omp.h>
#include "rng.h"

const float A=1;

using std::chrono::high_resolution_clock;
using std::chrono::duration;

// function signatures
float distance2collision(float mac_XS, float *x, float *y, float *z, const float r, const float u, const float v, const float w, bool *termination) ;
int   determine_reaction(const float sig_s, const float sig_a); 
void  sample_isotropic(float* u, float* v, float* w);
void  energy_angle_transfer(float* E, float* u, float* v, float* w);


// create one vector of all the tracks and just append in the looping section
// post process it with a reduce with multiple threads to do efficientlyl and avoid having
// many threads tyring to touch the same memory address

// this function is responsible for driving and timing the montecarlo simulation
int main(int argc, char* argv[]) {
    int num_histories = atoi(argv[1]); // number of simulations
    int threads = atoi(argv[2]); // number of threads

    omp_set_num_threads(threads);

    // timing variables for the history portion
    high_resolution_clock::time_point start_histories;
    high_resolution_clock::time_point end_histories; 
    duration<float, std::milli> duration_ms_histories;
    // timing variables for the estimator portion
    high_resolution_clock::time_point start_estimator;
    high_resolution_clock::time_point end_estimator;
    duration<float, std::milli> duration_ms_estimator;
    // total time
    duration<float, std::milli> duration_total;

    float r = 5.0f; // units in cm
    // cross sectin data
    const float sig_s = 0.9; // units in per cm
    const float sig_a = 0.1; // units in per cm
    const float sig_t = sig_s + sig_a;
    float mfp = 1/sig_t;
    float x,y,z = 0.0f; // position variables, units of cm
    float u,v,w,E = 0.0f;  // direction and energy
    bool terminate = false; // should simulation be terminated
    int rxn = 0;
    float score = 0.0f;

    std::vector<float> scores;

    // begin timing and parallel region
    start_histories = high_resolution_clock::now();
    #pragma omp parallel firstprivate(x,y,z,u,v,w,E,terminate,rxn,score)
    {
        std::vector<float> scores_private;
        #pragma omp for schedule(dynamic,1) // choosing dynamic,1 since each history has a stochastic number of tracks
        for(int i=0; i < num_histories; i++) {
            //flux_elem = 0.0f;
            score = 0.0f;
            terminate = false; // do not terminate simulation until a history-ending event occurs
            x = 0.0f ; y= 0.0f ; z=0.0f ; E=100.0f; // each new history starts at the origin with energy 100
            u = 0.0f ; v = 0.0f; w=0.0f;
            sample_isotropic(&u,&v,&w); // initial direction sampled from isotropic distribution
            while(!terminate) { 
                score += distance2collision(mfp,&x,&y,&z,r,u,v,w,&terminate); // this function modifies position and terminate, but not u,v,w
                if(terminate) {
                    // particle has escaped geometry as d2c modified terminate to be true, continue to next history
                    continue;
                } else {
                    // the particle history has not terminated by leaving the geometry
                    // sample reaction type
                    rxn = determine_reaction(sig_s,sig_a);
                    if(rxn==0) {
                        // scattering event
                        energy_angle_transfer(&E,&u,&v,&w); // determine outgoing direction and outgoing energy
                    } else {
                        // absorption event, history is terminated
                        terminate=true;
                    }
                }
            }
            scores_private.push_back(score);
        }
        #pragma omp critical
        {
        scores.insert(scores.end(), scores_private.begin(), scores_private.end());
        }
    } // end parallel region

    end_histories = high_resolution_clock::now();
    duration_ms_histories = std::chrono::duration_cast<duration < float, std::milli> > (end_histories - start_histories);

    start_estimator = high_resolution_clock::now();
    
    float flux = 0.0f; // flux estimator
    float RE = 0.0f; // relative error
    float V = 4/3*M_PI*r*r*r; // vollume
    float subtractor = 0.0f;

    for(int i = 0; i < num_histories; i++) {
        float score = scores[i];
        flux += score;
        RE += score * score;
        subtractor += score/num_histories;
    }

    flux /= num_histories*V;
    RE /= num_histories;  
    // square the subtractor
    subtractor *= subtractor;
    // correct RE
    RE -= subtractor;

    end_estimator = high_resolution_clock::now();
    duration_ms_estimator = std::chrono::duration_cast<duration < float, std::milli> > (end_estimator - start_estimator);

    std::cout << flux << "," << RE << "," << 
                 duration_ms_histories.count() << "," << 
                 duration_ms_estimator.count() << "," <<  
                 duration_ms_histories.count() + duration_ms_estimator.count() << std::endl;

    return 0;
}


// responsible for sampling exponential distribution and
// determining if the particle remains in the geometry
float distance2collision(float mac_XS, float *x, float *y, float *z, const float r, const float u, const float v, const float w, bool *termination) {
    // sample distance to event from exponential distribution
    float xi = gen_rand_0_to_1();
    float d = -log(xi)/mac_XS; // units of cm  
    // check to see if d is infinite. If so, return a distance that will always leave the sphere
    if(isinf(d) == 1) {
        d = 2*r + 1;
    } 
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
        // absorption event
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
// assumes mass number A is defined in the #include/#define section
void energy_angle_transfer(float* E, float* u, float* v, float* w) {
    // modify E,u,v,w in a safe way that gives new direction
    // (u,v,w) is incoming direciton
    // (U,V,W) is outgoing direction
    float E_in = *E; // store originaal energy 
    // generate a new isotropic direction
    float U;
    float V;
    float W;
    sample_isotropic(&U,&V,&W);
    // compute the dot product between incoming and exiting directions
    float mu = (*u) * (U) + (*w) * (W) + (*v) * (V);
    // elastic scattering physics
    float factor = pow((1/(1+A)),2)*(1+A*A + 2*A*mu) ; 
    // update direction and energy for next event
    *u = U;
    *v = V;
    *w = W;
    *E = E_in * factor;
}


