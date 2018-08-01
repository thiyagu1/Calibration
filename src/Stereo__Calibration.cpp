//
//  Stereo__Calibration.cpp
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 22/04/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

#include "Stereo__Calibration.hpp"


Stereo_calibrate:: Stereo_calibrate(int board_width, int board_height, int num_imgs, float square_size, const char *imgs_directory){
    
    Board_height = board_height;
    Board_width = board_width;
    Num_imgs = num_imgs;
    Square_size = square_size;
    Dir = imgs_directory;
    boardsize = Size(board_width,board_height);
    cout<<"intialized : "<<Board_height<<endl<<Board_width<<endl<<Num_imgs<<endl<<Square_size<<endl;
}

void Stereo_calibrate::Varjo_initialize(){
    
    bool Ok;
    for (int i=1; i<=Num_imgs; i++) {
        string Filename = Dir + std::to_string(i) + ".bmp";
        cout<<Filename<<endl;
        img = imread(Filename, CV_LOAD_IMAGE_COLOR);
        cv::cvtColor(img, grey, CV_BGR2GRAY);
        Ok = findChessboardCorners(img, boardsize, corners,CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS);
        if (Ok)
        {
            cornerSubPix(grey, corners, cv::Size(5, 5), cv::Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(grey, boardsize, corners, Ok);
        }
        //namedWindow( "View", WINDOW_AUTOSIZE );
        //imshow("View", grey);
        //waitKey(0);
        // Evaluating the corners in the Real world
        
        for (int j=0; j<Board_height; j++) {
            for (int k=0; k<Board_width; k++) {
                realw.push_back(Point3f((float)k * Square_size, (float)j * Square_size, 0));
            }
        }
        if(Ok)
        {
            cout<<i<<" Corners Found"<<endl;
            RealP.push_back(realw);
            ImageP.push_back(corners);
        }
        realw.clear();
        corners.clear();
    }
    
     //flag |= CV_CALIB_FIX_INTRINSIC;
//    printf("Starting Calibration\n");
//
//    flag |= CV_CALIB_FIX_K4;
//    flag |= CV_CALIB_FIX_K5;
//    calibrateCamera(RealP, ImageP, img.size(), K, D, rvecs, tvecs, flag);
//    distortion_correction(Dir, 15, K, D);
//    int kp = Dump_result(K, D, rvecs, tvecs);
//    if(kp == 1)
//    {
//        cout<<"Print was success"<<endl;
//    }
//    else
//        cout<<"Print failed"<<endl;
//

}

Mat Stereo_calibrate:: distortion_correction(char const* imgs_directory,int number, Mat camera_matrix, Mat Distortion_coeff){
    
    Mat img, imgk,ori, imgdis_corrected;
    string Filename = imgs_directory + std::to_string(number) + ".bmp";
    ori = imread(Filename, CV_LOAD_IMAGE_COLOR);
    cout<<Filename<<endl;
    imgk = imread(Filename, CV_LOAD_IMAGE_COLOR);
    undistort(imgk, imgdis_corrected, camera_matrix, Distortion_coeff);
  //  remap(imgk, imgdis_corrected, map1, map2, cv::INTER_LINEAR,cv::BORDER_CONSTANT, cv::Scalar());
    imwrite( "./Result/Distortion_Corrected.bmp", imgdis_corrected );
    imwrite( "./Result/Original_Distorted.bmp", ori);
    return imgdis_corrected;
}






int Stereo_calibrate:: Dump_result(Mat K, Mat D, vector< Mat > rvecs, vector< Mat > tvecs){
    
    cout<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"Camera Matrix : "<<endl;
    cout <<K<<endl;
    std::ofstream output_file6("A_Camera_Matrix.txt");
    output_file6 << K;
    output_file6.close();
    cout<<"*****************************************"<<endl;
    cout<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"Distortion Coefficient : "<<endl;
    cout<<"K1,K2,P1,P2,K3"<<endl;
    cout <<D<<endl;
    std::ofstream output_file5("A_Distortion_Coefficient.txt");
    output_file5 << D;
    output_file5.close();
    cout<<"*****************************************"<<endl;
    cout<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"Rotation Vector: "<<endl;
    cout <<rvecs[1]<<endl;
    std::ofstream output_file3("A_Rotation_vector.txt");
    std::ostream_iterator<Mat> output_iterator3(output_file3);
    std::copy(rvecs.begin(), rvecs.end(), output_iterator3);
    cout<<"*****************************************"<<endl;
    cout<<endl;
    cout<<"*****************************************"<<endl;
    cout<<"Translation Vector : "<<endl;
    cout <<tvecs[1]<<endl;
    std::ofstream output_file4("A_Translation_vector.txt");
    std::ostream_iterator<Mat> output_iterator4(output_file4, "\n");
    std::copy(tvecs.begin(), tvecs.end(), output_iterator4);
    cout<<"*****************************************"<<endl;
    return 1;
}

Stereo_calibrate::~Stereo_calibrate()
{
    std::cout << "Cature is freed" << std::endl;
}


