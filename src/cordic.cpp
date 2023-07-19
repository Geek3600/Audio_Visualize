//The file cordic.h holds definitions for the data types and constant valuse

#include "cordic.h"

//The cordic_phase array holds the angle for the current rotation
THETA_TYPE cordic_phase[NUM_ITERATIONS] = {
	0.7854, 0.4461, 0.2450, 0.1244,
	0.0624, 0.0312, 0.0156
};

void cordic(THETA_TYPE theta, COS_SIN_TYPE &s, COS_SIN_TYPE &c) {
    // Set the initial vector that we will rotate
    // current cos = I; current sin = Q
    COS_SIN_TYPE current_cos = 0.60735;
    COS_SIN_TYPE current_sin = 0.0;

    // This loop iteratively rotates the initial vector to ﬁnd the
    // sine and cosine values corresponding to the input theta angle
    for (int j = 0; j < NUM_ITERATIONS; j++) {
        // Multiply previous iteration by 2ˆ(-j). This is equivalent to
        // a right shift by j on a ﬁxed-point number.
        COS_SIN_TYPE cos_shift = current_cos >> j;
        COS_SIN_TYPE sin_shift = current_sin >> j;

        // Determine if we are rotating by a positive or negative angle
        if(theta >= 0) {
            // Perform the rotation
            current_cos = current_cos - sin_shift;
            current_sin = current_sin + cos_shift;

            // Determine the new theta
            theta = theta - cordic_phase[j];
        } else {
            // Perform the rotation
            current_cos = current_cos + sin_shift;
            current_sin = current_sin - cos_shift;

            // Determine the new theta
            theta = theta + cordic_phase[j];
        }
    }
    // Set the ﬁnal sine and cosine values
    s = current_sin;
    c = current_cos;
}
