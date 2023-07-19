#include "fft.h"

void fft_stage(int stage, DTYPE X_R[SIZE], DTYPE X_I[SIZE],
               DTYPE Out_R[SIZE], DTYPE Out_I[SIZE]){
    int DFTpts = 1 << stage; // DFT = 2^stage = points in sub DFT
    int numBF = DFTpts >> 1; // Butterfly WIDTHS in sub−DFT
    int step = SIZE >> stage;
    DTYPE k = 0;
    DTYPE e = -6.283185307178 / DFTpts;
    THETA_TYPE a[SIZE2];
    COS_SIN_TYPE s[SIZE2], c[SIZE2];
#pragma HLS array_partition variable=a cyclic factor=2 dim=1
#pragma HLS array_partition variable=s cyclic factor=2 dim=1
#pragma HLS array_partition variable=c cyclic factor=2 dim=1
	a[0] = 0;
calc_a_loop:
	for (int i = 1; i < numBF; i++){
		a[i] = a[i-1] + e;
	}
cordic_loop:
	for (int i = 0; i < numBF; i++){
		cordic(a[i], s[i], c[i]);
	}
    // Perform butterflies for j−th stage
butterfly_loop:
    for (int j = 0; j < numBF; j++){
        // Compute butterflies that use same W**k
	dft_loop:
        for (int i = j; i < SIZE; i += DFTpts){
#pragma HLS loop_tripcount min=1 max=8192
            int i_lower = i + numBF; // index of lower point in butterfly
            DTYPE temp_R = X_R[i_lower] * c[j] - X_I[i_lower] * s[j];
            DTYPE temp_I = X_I[i_lower] * c[j] + X_R[i_lower] * s[j];
            Out_R[i_lower] = X_R[i] - temp_R;
            Out_I[i_lower] = X_I[i] - temp_I;
            Out_R[i] = X_R[i] + temp_R;
            Out_I[i] = X_I[i] + temp_I;
        }
        k += step;
    }
}

void fft_streaming(int X[LEN_AUDIO+FFT_WINDOW_SIZE], int end, int OUT[SIZE])
{
	ap_fixed<16, 8> after_window[FFT_WINDOW_SIZE];
#pragma HLS array_partition variable=after_window cyclic factor=2 dim=1
	int frame[FFT_WINDOW_SIZE];
#pragma HLS array_partition variable=frame cyclic factor=2 dim=1
	//	origin: 0 now:WINDOW_SIZE
frame_loop:
	for (int i = 0; i < FFT_WINDOW_SIZE; i++){
		frame[i] = X[end + i];
	}
	add_window(frame, after_window);
	DTYPE X_R[SIZE];
#pragma HLS array_partition variable=X_R cyclic factor=2 dim=1
x_loop:
	for (int i = 0; i < FFT_WINDOW_SIZE; i++)
		X_R[i] = after_window[i];
zeros_loop:
	for (int i = FFT_WINDOW_SIZE; i < SIZE; i++)
		X_R[i] = 0;
	DTYPE Stage_R[M+1][SIZE], Stage_I[M+1][SIZE];
#pragma HLS ARRAY_PARTITION variable=Stage_R dim=1 complete
#pragma HLS ARRAY_PARTITION variable=Stage_I dim=1 complete

	bit_reverse(X_R, Stage_R[0], Stage_I[0]);
stage_loop:
	for (int stage = 1; stage <= M; stage++){
#pragma HLS unroll
		fft_stage(stage, Stage_R[stage-1], Stage_I[stage-1], Stage_R[stage], Stage_I[stage]);
	}
out_loop:
	for (int i = M; i < SIZE; i++){
		OUT[i] = (Stage_R[M][i] * Stage_R[M][i] + Stage_I[M][i] * Stage_I[M][i]) >> 17;
	}
}


