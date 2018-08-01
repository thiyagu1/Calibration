//
//  Mono__Calibration.hpp
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 23/04/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

#ifndef Mono__Calibration_hpp
#define Mono__Calibration_hpp

#include "VarjoVST.h"

class Mono_calibrate{
    
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
    Mono_calibrate(int board_width, int board_height, int num_imgs, float square_size, const char *imgs_directory);
    void Varjo_initialize();
    Mat distortion_correction(char const* imgs_directory,int number, Mat camera_matrix, Mat Distortion_coeff);
    int Dump_result(Mat K, Mat D, vector< Mat > rvecs, vector< Mat > tvecs);
     vector<double>  FOV(Mat , Size);
    ~Mono_calibrate();
};

#endif /* Mono__Calibration_hpp */
