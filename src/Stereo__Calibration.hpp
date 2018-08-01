//
//  Stereo__Calibration.hpp
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 22/04/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

#ifndef Stereo__Calibration_hpp
#define Stereo__Calibration_hpp
#include "VarjoVST.h"

class Stereo_calibrate{

    public:
        int Board_width, Board_height, Num_imgs;
        char const* Dir;
        float Square_size;
        vector< Point2f > corners;
        vector<Point3f> realw;
        vector<vector<Point3f> > RealP;
        vector<vector<Point2f> > ImageP;
        vector< Mat > rvecs, tvecs;  // Rotation and Translation Vectors
        Mat K; // Camera Matrix
        Mat D; // Distortion coefficient
        Mat img, grey;
        int flag = 0;
        Size boardsize;
    
    public:
        Stereo_calibrate(int board_width, int board_height, int num_imgs, float square_size, const char *imgs_directory);
        void Varjo_initialize();
        Mat distortion_correction(char const* imgs_directory,int number, Mat camera_matrix, Mat Distortion_coeff);
        int Dump_result(Mat K, Mat D, vector< Mat > rvecs, vector< Mat > tvecs);
        ~Stereo_calibrate();

};

#endif /* Stereo__Calibration_hpp */
