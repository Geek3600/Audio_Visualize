#ifndef __FFT_H__
#define __FFT_H__
#include "cordic.h"
#include "bit_reverse.h"
#include "window.h"

#define M FFT_BITS
#define LEN_AUDIO 614466

typedef ap_uint<512> MEM_TYPE;

void fft_stage(int stage, DTYPE X_R[SIZE], DTYPE X_I[SIZE],
               DTYPE Out_R[SIZE], DTYPE Out_I[SIZE]);

void fft_streaming(int X[LEN_AUDIO+FFT_WINDOW_SIZE], int end, int OUT[SIZE]);

#endif
