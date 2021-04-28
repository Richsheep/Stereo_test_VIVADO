#include "stereo_top.h"
#include "hls_opencv.h"

using namespace hls;

int main (int argc, char** argv) 
{
    // 图像加载
    IplImage* src_l = cvLoadImage("tsukuba_l.png", CV_LOAD_IMAGE_COLOR);
    IplImage* src_r = cvLoadImage("tsukuba_r.png", CV_LOAD_IMAGE_COLOR);
    
    // 输出图像初始化
    IplImage* dst = cvCreateImage(cvGetSize(src_l), IPL_DEPTH_8U, 3);

    AXI_STREAM_IN src_l_axi;
    AXI_STREAM_IN src_r_axi;
    
    AXI_STREAM_OUT dst_axi;

    // 将图像转换为AXI视频流结构
    IplImage2AXIvideo(src_l, src_l_axi); 
    IplImage2AXIvideo(src_r, src_r_axi);
    
    // 计算深度图
    hls_stereoBM(src_l_axi, src_r_axi, dst_axi, src_l->height, src_r->width);
	
    // AXI视频流结构转换为图像
    AXIvideo2IplImage(dst_axi, dst);

    // 保存结果图像
    cvSaveImage("OUTPUT_IMAGE.png", dst);

    cvReleaseImage(&src_l);
    cvReleaseImage(&src_r);
    cvReleaseImage(&dst);
}
