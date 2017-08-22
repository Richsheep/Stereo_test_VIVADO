#ifndef __CALIBRATE_H__
#define __CALIBRATE_H__
#include "hls_video.h"
#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080

typedef hls::stream<ap_axiu<24,1,1,1> > AXI_STREAM_IN;
typedef hls::stream<ap_axiu<24,1,1,1> > AXI_STREAM_OUT;

typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3> RGB_IMAGE;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC1> GRAY_IMAGE;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_16SC2> CALIB_MAP;

typedef hls::Scalar<3, unsigned char> RGB_PIXEL;
typedef hls::Scalar<1, unsigned char> GRAY_PIXEL;
typedef hls::Scalar<1, char> DISP_PIXEL;

/*
map1 and map2 are only supported as HLS_16SC2. cameraMatrix, and newCameraMatrix,
are normalized in the sense that their form is:
[f_x,0,c_x,
0,f_y,c_y,
0,0,1]

R and ir are also normalized with the form:
[a,b,c,
d,e,f,
0,0,1]
*/

void hls_calib(AXI_STREAM_IN& uncalib_l, AXI_STREAM_IN& uncalib_r, AXI_STREAM_OUT& rectified_l, AXI_STREAM_OUT& rectified_r,int rows, int cols);

#endif
