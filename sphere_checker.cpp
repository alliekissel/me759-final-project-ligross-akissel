#include <iostream>
#include <cmath>

int main() {
    float x=1,y=0,z=0;
    float u=1/sqrt(2), v=1/sqrt(2), w = 0;
    float r = 5.0f;
    float d = 8.0f;
    float mag_A_sq = (x)*(x)+(y)*(y)+(z)*(z);
    float mag_A = sqrt(mag_A_sq);
    // the magnitude of the vector for the point the particle is transported to
    float mag_end_sq = (x+d*u)*(x+d*u)+(y+d*v)*(y+d*v)+(z+d*w)*(z+d*w);
    std::cout << "ending point mag sq " << mag_end_sq << std::endl;
    float costheta = -(mag_end_sq - mag_A_sq - d*d)/(2*mag_A*d);  // T0DO_LG cook up some examples where we know costheta to double check its right
    std::cout << "mag A sq " << mag_A_sq << std::endl;
    std::cout << "mag end sq " << mag_end_sq << std::endl;
    std::cout << costheta << std::endl;
    // determine d_i from quadratic
    float a,b,c;
    a = 1.0f;
    b = -2*mag_A*costheta;
    c = mag_A_sq - r*r;
    float d_i_plus = (-b + sqrt(b*b- 4*a*c))/(2*a);
    float d_i_minus = (-b - sqrt(b*b - 4*a*c))/(2*a);
    std::cout << "plus " << d_i_plus << std::endl;
    std::cout << "minus " << d_i_minus << std::endl;

    float r_plus_sq = (x+d_i_plus*u)*(x+d_i_plus*u)+(y+d_i_plus*v)*(y+d_i_plus*v)+(z+d_i_plus*w)*(z+d_i_plus*w);
    float r_minus_sq = (x+d_i_minus*u)*(x+d_i_minus*u)+(y+d_i_minus*v)*(y+d_i_minus*v)+(z+d_i_minus*w)*(z+d_i_minus*w);

    std::cout << "r_plus_sq is " << r_plus_sq << " and r_minus_sq is " << r_minus_sq << " compated to r^2 " << r*r << std::endl;

    // check if A + d_i Omega = B, i.e. on sphere before assigning d_i, maybe add function for this?
    float d_i;

    return 0;
}