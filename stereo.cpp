#include "stereo_top.h"

using namespace hls;

void hls_stereoBM(AXI_STREAM_IN& input_l, AXI_STREAM_IN& input_r, AXI_STREAM_OUT& disp_axi, int rows, int cols) 
{
    #pragma HLS RESOURCE variable=input_l core=AXI4Stream metadata="-bus_bundle INPUT_STREAM_l"
    #pragma HLS RESOURCE variable=input_r core=AXI4Stream metadata="-bus_bundle INPUT_STREAM_r"
    #pragma HLS RESOURCE variable=disp_axi core=AXI4Stream metadata="-bus_bundle OUTPUT_STREAM"

    #pragma HLS INTERFACE ap_stable port = cols
    #pragma HLS INTERFACE ap_stable port = rows
    #pragma HLS interface ap_ctrl_none port=return

    RGB_IMAGE img_l(rows, cols);
    RGB_IMAGE img_r(rows, cols);

    Gray_IMAGE img_l_gray(rows, cols);
    Gray_IMAGE img_r_gray(rows, cols);
  
    DISP_IMAGE img_disp(rows, cols);
  
    Gray_IMAGE img_disp_8u(rows, cols);
  
    RGB_IMAGE img_out(rows, cols);
  
    BMState BM;
    BM.uniquenessRatio = 25;
  
    #pragma HLS dataflow
  
    AXIvideo2Mat(input_l, img_l); //read video stream by frames:left
    AXIvideo2Mat(input_r, img_r); //read video stream by frames:right
  
    CvtColor<HLS_RGB2GRAY>(img_l, img_l_gray);
    CvtColor<HLS_RGB2GRAY>(img_r, img_r_gray);
  
    FindStereoCorrespondenceBM(img_l_gray, img_r_gray, img_disp, BM);
  
    ConvertScaleAbs(img_disp, img_disp_8u, 1);
  
    CvtColor<HLS_GRAY2RGB>(img_disp_8u, img_out);
  
    Mat2AXIvideo(img_out, disp_axi); //write the frames to video stream
}
