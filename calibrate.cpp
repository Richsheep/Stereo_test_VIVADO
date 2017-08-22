#include "calibrate.h"

using namespace hls;


void hls_calib(AXI_STREAM_IN& uncalib_l, AXI_STREAM_IN& uncalib_r, AXI_STREAM_OUT& rectified_l, AXI_STREAM_OUT& rectified_r,int rows, int cols) {


#pragma HLS RESOURCE variable=uncalib_l core=AXI4Stream metadata="-bus_bundle INPUT_STREAM_l"
#pragma HLS RESOURCE variable=rectified_l core=AXI4Stream metadata="-bus_bundle OUTPUT_STREAM_l"
#pragma HLS RESOURCE variable=uncalib_r core=AXI4Stream metadata="-bus_bundle INPUT_STREAM_r"
#pragma HLS RESOURCE variable=rectified_r core=AXI4Stream metadata="-bus_bundle OUTPUT_STREAM_r"

#pragma HLS INTERFACE ap_stable port = cols
#pragma HLS INTERFACE ap_stable port = rows
//AP_CONTROL_BUS_AXI(CONTROL_BUS);
#pragma HLS interface ap_ctrl_none port=return

RGB_IMAGE img_uncalib_l(rows, cols);
RGB_IMAGE img_uncalib_r(rows, cols);

RGB_IMAGE img_out_l(rows, cols);
RGB_IMAGE img_out_r(rows, cols);

CALIB_MAP Map_x_l(rows, cols);
CALIB_MAP Map_x_r(rows, cols);
CALIB_MAP Map_y_l(rows, cols);
CALIB_MAP Map_y_r(rows, cols);

Window<3, 3, double> CameraMatrix_l;
Window<3, 3, double> CameraMatrix_r;
Window<3, 3, double> CameraMatrix_new;
Window<3, 3, double> R_l;
Window<3, 3, double> R_r;
Window<3,3, double> iR_l;
Window<3,3, double> ir_l;
Window<3,3, double> iR_r;
Window<3,3, double> ir_r;

//Map_x_l = ;

const double oldcam_l[3][3] = {{564.38807, 0, 331.45142},
   					{0, 562.63442,  228.75790},
   					{0, 0, 1}};
const double oldcam_r[3][3] = {{567.12884, 0, 300.83065},
   					{0, 565.27532,  225.12485},
   					{0, 0, 1}};

const double newcam[3][3] = {{544.1768093525974, 0, 318.7513427734375},
					{0, 544.1768093525974, 228.7405624389648},
					{0, 0, 1}};

const double dist_l[5] {-0.37892 ,  0.17973  , 0.00316 ,  -0.00058 , 0.00000};

const double dist_r[5] {-0.38948 ,  0.19991 ,  0.00321 ,  -0.00075 , 0.00000};

const double Rl[3][3] = 	{{0.9998055589300135, 0.01956925038649927, -0.002426679202877205},
					{-0.01958491736455809, 0.999786347919893, -0.006609805203364947},
					{0.002296811804787115, 0.00665604629742744, 0.9999752105443513}};

const double Rr[3][3] = 	{{0.9998675197903206, 0.01395142000939706, -0.008384554136873387},
					{-0.01389549438378709, 0.999881060949387, -0.006691725529227457},
					{0.008476915959410383, 0.006574331483106749, 0.9999424583751644}};


for (int i = 0; i < 3; i++)
{
	for (int j = 0; j < 3; j++)
	{
		CameraMatrix_l.val[i][j] = oldcam_l[i][j];
		CameraMatrix_r.val[i][j] = oldcam_r[i][j];
		CameraMatrix_new.val[i][j] = newcam[i][j];
		std::cout << CameraMatrix_new.val[i][j] ;
		R_l.val[i][j] = Rl[i][j];
		R_r.val[i][j]= Rr[i][j];
	}
}
mul<3, 3, 3>(CameraMatrix_new.val, R_l.val, iR_l.val);
Invert<3>(iR_l.val, ir_l.val);
mul<3, 3, 3>(CameraMatrix_new.val, R_r.val, iR_r.val);
Invert<3>(iR_r.val, ir_r.val);


#pragma HLS dataflow
AXIvideo2Mat(uncalib_l, img_uncalib_l); //read video stream uncalibrate
AXIvideo2Mat(uncalib_r, img_uncalib_r); //read video stream uncalibrate
/*
InitUndistortRectifyMap(CameraMatrix_l, dist_l, R_l, CameraMatrix_new, Map_x_l, Map_y_l);
InitUndistortRectifyMap(CameraMatrix_r, dist_r, R_r, CameraMatrix_new, Map_x_r, Map_y_r);
*/


InitUndistortRectifyMapInverse(CameraMatrix_l, dist_l, ir_l, Map_x_l, Map_y_l, true);
InitUndistortRectifyMapInverse(CameraMatrix_r, dist_r, ir_r, Map_x_r, Map_y_r, true);

/*
template< typename CMT, typename RT, typename DT, int ROWS, int COL, int MAP1_T, int MAP2_T, int N>
void InitUndistortRectifyMap(
                            Window<3,3, CMT> cameraMatrix,
                            DT (&distCoeffs)[N],
                            Window<3,3, RT> R,
                            Window<3,3, CMT> newcameraMatrix,
                            Mat<ROWS, COL, MAP1_T>  &map1,
                            Mat<ROWS, COL, MAP2_T>  &map2
                            )
{
    Window<3,3, RT> iR;
    Window<3,3, RT> ir;
    mul<3, 3, 3>(newcameraMatrix.val, R.val, iR.val);
    Invert<3>(iR.val, ir.val);
    InitUndistortRectifyMapInverse(cameraMatrix, distCoeffs,ir,map1,map2);
}
*/

Remap<256>(img_uncalib_l, img_out_l, Map_x_l, Map_y_l, HLS_INTER_NEAREST);
Remap<256>(img_uncalib_r, img_out_r, Map_x_r, Map_y_r, HLS_INTER_NEAREST);

//WINROW cannot be 0 and 480

/*
template <int WIN_ROW, int ROW, int COLS, int SRC_T, int DST_T, int MAP1_T, int MAP2_T>

void Remap(
           Mat<ROW, COLS, SRC_T>    &src,
           Mat<ROW, COLS, DST_T>    &dst,
           Mat<ROW, COLS, MAP1_T>   &map1,
           Mat<ROW, COLS, MAP2_T>   &map2,
           int interpolation=HLS_INTER_NEAREST
        //int bordertype=BORDER_CONSTANT
        //const Scalar<HLS_MAT_CN(SRC_T),HLS_NAME_MAT(SRC_T)> borderValue
        )
{
    if(interpolation==HLS_INTER_NEAREST) {
        Remap_nearest<WIN_ROW>(src, dst, map1, map2);
    } else if(interpolation==HLS_INTER_LINEAR) {
        Remap_linear<WIN_ROW>(src, dst, map1, map2);
    }
}
*/

Mat2AXIvideo(img_out_l, rectified_l); //write the frames to video stream
Mat2AXIvideo(img_out_r, rectified_r); //write the frames to video stream

}
