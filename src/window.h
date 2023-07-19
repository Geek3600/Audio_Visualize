#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "cordic.h"

#define FFT_WINDOW_SIZE 11024

void add_window(int IN[FFT_WINDOW_SIZE], ap_fixed<16, 8> OUT[FFT_WINDOW_SIZE]);

#endif
