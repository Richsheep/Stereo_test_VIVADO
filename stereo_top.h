#ifndef _STEREO_TOP_H_
#define _STEREO_TOP_H_
#include "hls_video.h"
#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080
#define INPUT_IMAGE_L "tsukuba_l.png"
#define INPUT_IMAGE_R "tsukuba_r.png"
#define OUTPUT_IMAGE "result_1080p.jpg"
#define OUTPUT_IMAGE_GOLDEN "result_1080p_golden.jpg"
#define WSIZE 5
#define NDISP 32
#define NDISP_UNIT 16

typedef hls::stream<ap_axiu<24,1,1,1> > AXI_STREAM_IN;
typedef hls::stream<ap_axiu<24,1,1,1> > AXI_STREAM_OUT;

typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3> RGB_IMAGE;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> Gray_IMAGE;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_16SC1> DISP_IMAGE;
typedef hls::StereoBMState<WSIZE, NDISP, NDISP_UNIT> BMState;
typedef hls::Scalar<3, unsigned char> RGB_PIXEL;
typedef hls::Scalar<1, unsigned char> GRAY_PIXEL;
typedef hls::Scalar<1, char> DISP_PIXEL;

void hls_stereoBM(AXI_STREAM_IN& input_l,AXI_STREAM_IN& input_r, AXI_STREAM_OUT& disp_axi, int rows, int cols);

#endif
