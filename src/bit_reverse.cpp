#include "bit_reverse.h"

unsigned int reverse_bits(unsigned int input) {
    int i, rev = 0;
    for (i = 0; i < FFT_BITS; i++) {
        rev = (rev << 1) | (input & 1);
        input = input >> 1;
    }
    return rev;
}

void bit_reverse(DTYPE X_R[SIZE],
                 DTYPE Out_R[SIZE], DTYPE Out_I[SIZE]) {
    unsigned int reversed;
    unsigned int i;

    for (i = 0; i < SIZE; i++) {
#pragma HLS dependence variable=Out_R inter false
        reversed = reverse_bits(i); // Find the bit reversed index
        if (i < reversed) {
            // Swap the real values
            Out_R[i] = X_R[reversed];
            Out_R[reversed] = X_R[i];
        }
        Out_I[i] = 0;
    }
}
