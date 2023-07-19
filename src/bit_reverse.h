#ifndef __BIT_REVERSE_H__
#define __BIT_REVERSE_H__
#include "ap_fixed.h"

#define FFT_BITS 14            // Number of bits of FFT, i.e., log(1024)
#define SIZE 16384                // SIZE OF FFT
#define SIZE2 SIZE >> 1 // SIZE/2
typedef ap_fixed<32, 24> DTYPE;

unsigned int reverse_bits(unsigned int input);

void bit_reverse(DTYPE X_R[SIZE],
                 DTYPE Out_R[SIZE], DTYPE Out_I[SIZE]);
#endif
