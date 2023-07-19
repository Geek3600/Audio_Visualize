#include "fft_warp.h"

void fft_warp(MEM_TYPE *X, MEM_TYPE *Y){
#pragma HLS INTERFACE m_axi port=X offset=slave bundle=gmem0 depth=38404
#pragma HLS INTERFACE m_axi port=Y offset=slave bundle=gmem1 depth=428032
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

#pragma HLS dataflow
	int X_local[LEN_AUDIO+FFT_WINDOW_SIZE];
#pragma HLS array_partition variable=X_local cyclic factor=2 dim=1
window_loop:
	for (int i = 0; i < FFT_WINDOW_SIZE; i++)
		X_local[i] = 0;
//	加上窗长避免分帧时越界
x_local_loop:
	for (int i = 0; i < LEN_AUDIO/16; i++){
		MEM_TYPE data = X[i];
		for (int ii = 0; ii < 16; ii++){
			X_local[FFT_WINDOW_SIZE + i * 16 + ii] = data.range(0 + (ii * 32), 31 + (ii * 32));
		}
	}
	int end[FRAME_COUNT];
#pragma HLS array_partition variable=end complete dim=1
	end[0] = 0;
end_loop:
	for (int i = 1; i < FRAME_COUNT; i++){
		end[i] = end[i - 1] + FRAME_OFFSET;
	}
	int OUT[FRAME_COUNT][SIZE];
#pragma HLS array_partition variable=OUT complete dim=1
fft_loop:
	for (int i = 0; i < FRAME_COUNT; i++){
#pragma HLS unroll
		fft_streaming(X_local, end[i], OUT[i]);
	}
	int cnt = 0;
y_loop:
	for (int i = 0; i < FRAME_COUNT*SIZE/16; i++){
		MEM_TYPE data;
		for (int ii = 0; ii < 16; i++){
			data.range(0 + (ii * 32), 31 + (ii * 32)) = *OUT[i * 16 + ii];
		}
		Y[i] = data;
	}
}
