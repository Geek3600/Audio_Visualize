#include "window.h"

void add_window(int IN[FFT_WINDOW_SIZE], ap_fixed<16, 8> OUT[FFT_WINDOW_SIZE]){
#pragma HLS array_partition variable=IN cyclic factor=2 dim=1
//	由于cordic会产生一定倍数的放缩，计算"1"
	THETA_TYPE theta = 0;
	COS_SIN_TYPE one, zero;
	cordic(theta, zero, one);
	THETA_TYPE e = 6.283185307178 / (FFT_WINDOW_SIZE - 1);
	THETA_TYPE a[FFT_WINDOW_SIZE];
	COS_SIN_TYPE s[FFT_WINDOW_SIZE], c[FFT_WINDOW_SIZE];
#pragma HLS array_partition variable=a cyclic factor=2 dim=1
#pragma HLS array_partition variable=s cyclic factor=2 dim=1
#pragma HLS array_partition variable=c cyclic factor=2 dim=1
	a[0] = 0;
calc_a_loop:
	for (int i = 1; i < FFT_WINDOW_SIZE; i++){
		a[i] = a[i-1] + e;
	}
cordic_loop:
	for (int i = 0; i < FFT_WINDOW_SIZE; i++){
		cordic(a[i], s[i], c[i]);
	}
	ap_fixed<11, 3> window[FFT_WINDOW_SIZE];
#pragma HLS array_partition variable=window cyclic factor=2 dim=1
out_loop:
	for (int i = 0; i < FFT_WINDOW_SIZE; i++){
		window[i] = one - c[i];
		OUT[i] = (window[i] * IN[i]) >> 27;
	}
}
