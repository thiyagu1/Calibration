//
//  Fisheye_calib.hpp
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 04/05/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

#ifndef Fisheye_calib_hpp
#define Fisheye_calib_hpp

#include "VarjoVST.h"

class Fisheye_calibrate{
    
private:
    int Board_width, Board_height, Num_imgs;
    char const* Dir;
    float Square_size;
    vector< Point2f > corners;
    vector<Point3f> realw;
    vector<vector<Point3f> > RealP;
    vector<vector<Point2f> > ImageP;
    vector<double > aFOV;
    vector< Mat > rvecs, tvecs;  // Rotation and Translation Vectors
    Mat K; // Camera Matrix
    Mat D; // Distortion coefficient
    Mat img, grey;
    int flag = 0;
    Size boardsize;
    
public:
    Fisheye_calibrate(int board_width, int board_height, int num_imgs, float square_size, const char *imgs_directory);
    void Varjo_initialize();
    Mat distortion_correction(char const* imgs_directory,int number, Mat camera_matrix, Mat Distortion_coeff, Size imageSize);
    int Dump_result(Mat K, Mat D, vector< Mat > rvecs, vector< Mat > tvecs);
    vector<double>  FOV(Mat , Size);
    ~Fisheye_calibrate();

};
#endif /* Fisheye_calib_hpp */
