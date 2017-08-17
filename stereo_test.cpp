#include "stereo_top.h"
//#include "opencv/cv.h"
//#include "opencv/cxcore.h"
//#include "opencv/highgui.h"
#include "hls_opencv.h"

using namespace hls;

int main (int argc, char** argv) {
    IplImage* src_l = cvLoadImage("tsukuba_l.png", CV_LOAD_IMAGE_COLOR);
	IplImage* src_r = cvLoadImage("tsukuba_r.png", CV_LOAD_IMAGE_COLOR);
    //IplImage* dst = cvCreateImage(cvGetSize(src_l), src_l->depth, src_l->nChannels);
    IplImage* dst = cvCreateImage(cvGetSize(src_l), IPL_DEPTH_8U, 3);
    AXI_STREAM_IN src_l_axi;
	AXI_STREAM_IN src_r_axi;
    AXI_STREAM_OUT dst_axi;
    IplImage2AXIvideo(src_l, src_l_axi); //将图像转换为视频流结构
	IplImage2AXIvideo(src_r, src_r_axi);
    hls_stereoBM(src_l_axi, src_r_axi, dst_axi, src_l->height, src_r->width);
    AXIvideo2IplImage(dst_axi, dst);
    cvSaveImage("OUTPUT_IMAGE.png", dst);
	//cvShowImage("Disparity", dst);

    cvReleaseImage(&src_l);
    cvReleaseImage(&src_r);
    cvReleaseImage(&dst);
}
