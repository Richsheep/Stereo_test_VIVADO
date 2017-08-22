#include "calibrate.h"
//#include "opencv/cv.h"
//#include "opencv/cxcore.h"
//#include "opencv/highgui.h"
#include "hls_opencv.h"

using namespace hls;

int main (int argc, char** argv) {
    IplImage* src_l = cvLoadImage("left1.bmp", CV_LOAD_IMAGE_COLOR);
	IplImage* src_r = cvLoadImage("right1.bmp", CV_LOAD_IMAGE_COLOR);
    //IplImage* dst = cvCreateImage(cvGetSize(src_l), src_l->depth, src_l->nChannels);
    IplImage* dst_l = cvCreateImage(cvGetSize(src_l), IPL_DEPTH_8U, 3);
    IplImage* dst_r = cvCreateImage(cvGetSize(src_l), IPL_DEPTH_8U, 3);

    AXI_STREAM_IN src_l_axi;
	AXI_STREAM_IN src_r_axi;
    AXI_STREAM_OUT dst_l_axi;
    AXI_STREAM_OUT dst_r_axi;
    IplImage2AXIvideo(src_l, src_l_axi); //将图像转换为视频流结构
	IplImage2AXIvideo(src_r, src_r_axi);

    hls_calib(src_l_axi, src_r_axi, dst_l_axi, dst_r_axi, src_l->height, src_r->width);

    AXIvideo2IplImage(dst_l_axi, dst_l);
    AXIvideo2IplImage(dst_r_axi, dst_r);
    cvSaveImage("rectifid_l.png", dst_l);
    cvSaveImage("rectifid_r.png", dst_r);
	//cvShowImage("Disparity", dst);

    cvReleaseImage(&src_l);
    cvReleaseImage(&src_r);
    cvReleaseImage(&dst_l);
    cvReleaseImage(&dst_r);
}
