#ifndef __CORDIC_H__
#define __CORDIC_H__

#include "ap_fixed.h"

#define NUM_ITERATIONS 7
typedef ap_fixed<10, 2> THETA_TYPE;
typedef ap_fixed<12, 4> COS_SIN_TYPE;

void cordic(THETA_TYPE theta, COS_SIN_TYPE &s, COS_SIN_TYPE &c);

#endif
