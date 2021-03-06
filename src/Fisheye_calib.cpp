//
//  Fisheye_calib.cpp
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 04/05/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

#include "Fisheye_calib.hpp"

void drawAxis(cv::Mat &_image, cv::InputArray _cameraMatrix, cv::InputArray _distCoeffs,
              cv::InputArray _rvec, cv::InputArray _tvec, float length) {
    // project axis points
    std::vector< cv::Point3f > axisPoints;
    axisPoints.push_back(cv::Point3f(0, 0, 0));
    axisPoints.push_back(cv::Point3f(length, 0, 0));
    axisPoints.push_back(cv::Point3f(0, length, 0));
    axisPoints.push_back(cv::Point3f(length, length, 0));
    std::vector< cv::Point2f > imagePoints;
    cv::projectPoints(axisPoints, _rvec, _tvec, _cameraMatrix, _distCoeffs, imagePoints);
    
    // draw axis lines
    cv::line(_image, imagePoints[0], imagePoints[1], cv::Scalar(0, 0, 255), 3);
    cv::line(_image, imagePoints[0], imagePoints[2], cv::Scalar(0, 255, 0), 3);
    cv::line(_image, imagePoints[0], imagePoints[3], cv::Scalar(255, 0, 0), 3);
}

Fisheye_calibrate:: Fisheye_calibrate(int board_width, int board_height, int num_imgs, float square_size, const char *imgs_directory){
    
    Board_height = board_height;
    Board_width = board_width;
    Num_imgs = num_imgs;
    Square_size = square_size;
    Dir = imgs_directory;
    boardsize = Size(board_width,board_height);
    // cout<<"intialized : "<<Board_height<<endl<<Board_width<<endl<<Num_imgs<<endl<<Square_size<<endl;
}

void Fisheye_calibrate::Varjo_initialize(){
    
    double W = Width;
    double H = Height;
    cv::Size imageSize(W,H);
    bool Ok;
    for (int i=1; i<=Num_imgs; i++) {
        string Filename = Dir + std::to_string(i) + EXT;
        //cout<<Filename<<endl;
        img = imread(Filename, CV_LOAD_IMAGE_COLOR);
        cv::cvtColor(img, grey, CV_BGR2GRAY);
        Ok = findChessboardCorners(img, boardsize, corners,CV_CALIB_CB_ADAPTIVE_THRESH|CV_CALIB_CB_FILTER_QUADS);
        if (Ok)
        {
            cornerSubPix(grey, corners, cv::Size(5, 5), cv::Size(-1, -1),
                         TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(grey, boardsize, corners, Ok);
        }
        
        if (View == 1)
        {
            namedWindow( "View", WINDOW_AUTOSIZE );
            imshow("View", grey);
            waitKey(0);
        }

        // Evaluating the corners in the Real world
        
        for (int j=0; j<Board_height; j++) {
            for (int k=0; k<Board_width; k++) {
                realw.push_back(Point3f((float)k * Square_size, (float)j * Square_size, 0));
            }
        }
        if(Ok)
        {
            cout<<" Image Number "<<i<<endl;
            RealP.push_back(realw);
            ImageP.push_back(corners);
        }
        
  
        
        realw.clear();
        corners.clear();
        
    }
    printf("Starting Calibration\n");
    
    int flag = 0;
    flag |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flag |= cv::fisheye::CALIB_CHECK_COND;
    flag |= cv::fisheye::CALIB_FIX_SKEW;

    //calibrateCamera(RealP, ImageP, img.size(), K, D, rvecs, tvecs, flag);
    double mse= fisheye::calibrate(RealP, ImageP, img.size(), K, D, rvecs, tvecs, flag,  cv::TermCriteria(3, 20, 1e-6));
    
//    Mat matImg;
//    cv::solvePnP(RealP, ImageP, K, D, rvecs, tvecs);
//    drawAxis(matImg, K, D, rvecs, tvecs, 90);
//
//    cv::imshow("Camera", matImg);
//    char c = cv::waitKey(30);
    
    
    cout<<"Before K and D : "<<endl<<K<<endl<<D<<endl;
    cout<<"Mean Square Error = "<<mse<<endl;
    //      Accessing elements in a matrix
    //
    //    cout<<"CCCCCCCCCCCCCCCCCCCC"<<endl;
    //
    //    D.at<double>(0,2)=0;
    //    D.at<double>(0,3)=0;
    //    cout << D.at<double>(0,0)<<endl;
    //     cout << D.at<double>(0,1)<<endl;
    //     cout << D.at<double>(0,2)<<endl;
    //    cout << D.at<double>(0,3)<<endl;
    //    cout << D.at<double>(0,4)<<endl;
    //    cout<<"CCCCCCCCCCCCCCCCCCCC"<<endl;
    
    //
    //    K.at<double>(0,0) = 389.8241046111801; //Focal x;
    //    K.at<double>(1,1) = 389.6369542541428; // Focal y;
    //    K.at<double>(0,2) = 329.436106017577; //Principal point x;
    //    K.at<double>(1,2) = 241.2532898346628;//Principal point y;
    //     D.at<double>(0,0) = -0.3041386201741061; // K1 Radial
    //     D.at<double>(0,1) = 0.2302673677725616; // K2 Radial
    //     D.at<double>(0,2) = -0.0008855646072833829; // P1 Tangential
    //     D.at<double>(0,3) = -0.000499746501428866; // P2 Tangential
    //      D.at<double>(0,4) = -0.152783926401833; // K3 Radial
    //
    //
    //    cout<<K;
    //    cout<<D;
    
    
    struct intrinsics
    {
        float K1;
        float K2;
        float K3;
        float K4;
        float P1;
        float P2;
        float Fx;
        float Fy;
        float Cx;
        float Cy;
        float aspectRatio;
        float padding;
        
    } intr;
    
    
    
    intr.K1 = D.at<double>(0,0);
    intr.K2 = D.at<double>(0,1);
    intr.K3 = D.at<double>(0,4);
    intr.K4 = 0.0;
    intr.P1 = D.at<double>(0,2);
    intr.P2 = D.at<double>(0,3);
    intr.Fx = K.at<double>(0,0)/W;
    intr.Fy = K.at<double>(1,1)/H;
    intr.Cx = K.at<double>(0,2)/W;
    intr.Cy = K.at<double>(1,2)/H;
    
    intr.aspectRatio = (double) W/H;
    cout<<"Aspect Ratio = "<<intr.aspectRatio<<endl;
    std::ofstream outFile("intrinsics.raw", std::ios::binary);
    outFile.write((const char*)&intr, sizeof(intrinsics));
    
    int kp = Dump_result(K, D, rvecs, tvecs);
    int number = Distortion_Image_No;
    distortion_correction(Dir, number, K, D, imageSize);
    FOV(K,imageSize);
    cout<<"FOV X= "<<aFOV[0]<<endl;
    cout<<"FOV Y= "<<aFOV[1]<<endl;
   
    
    
    if(kp == 1)
    {
        cout<<"Print was success"<<endl;
    }
    else
        cout<<"Print failed"<<endl;
}
///Users/thiyaga/Documents/thiy/Codes/Camera_Test/build/Debug/VLeftBig/1.jpg

Mat Fisheye_calibrate:: distortion_correction(char const* imgs_directory,int number, Mat camera_matrix, Mat Distortion_coeff, Size imageSize){
   
    Mat newK, map1, map2;
    Mat img, imgk,ori, imgdis_corrected;
    string Filename = imgs_directory + std::to_string(number) + EXT;
    ori = imread(Filename, CV_LOAD_IMAGE_COLOR);
    cout<<Filename<<endl;
    imgk = imread(Filename, CV_LOAD_IMAGE_COLOR);
    //fisheye::undistortImage(imgk, imgdis_corrected, camera_matrix, Distortion_coeff);
    fisheye::estimateNewCameraMatrixForUndistortRectify(camera_matrix, Distortion_coeff, imageSize, Matx33d::eye(), newK, 1);
    fisheye::initUndistortRectifyMap(K, D, Matx33d::eye(), newK, imageSize, CV_16SC2, map1, map2);
    remap(imgk, imgdis_corrected, map1, map2, INTER_LINEAR);
    imwrite( "./Result/Distortion_Corrected.jpg", imgdis_corrected );
    imwrite( "./Result/Original_Distorted.jpg", ori);
    return imgdis_corrected;
}

int Fisheye_calibrate:: Dump_result(Mat K, Mat point, vector< Mat > rvecs, vector< Mat > tvecs){
    
    cout<<"Check List : "<<endl;
    std::ofstream output_file6("./Mono/A_Camera_Matrix.txt");
    output_file6 << K;
    output_file6.close();
    cout<<"1. Camera Matrix Completed "<<endl;
    
    
    std::ofstream output_file5("./Mono/A_Distortion_Coefficient.txt");
    cout<<D;
    output_file5 << D;
    output_file5.close();
    cout<<"2. Distortion Coefficient Completed "<<endl;
    
    
    std::ofstream output_file3("./Mono/A_Rotation_vector.txt");
    std::ostream_iterator<Mat> output_iterator3(output_file3);
    std::copy(rvecs.begin(), rvecs.end(), output_iterator3);
    output_file3.close();
    cout<<"3. Rotation Vector Completed "<<endl;
    
    
    
    std::ofstream output_file4("./Mono/A_Translation_vector.txt");
    std::ostream_iterator<Mat> output_iterator4(output_file4, "\n");
    std::copy(tvecs.begin(), tvecs.end(), output_iterator4);
    output_file4.close();
    cout<<"4. Translation Vector Completed "<<endl;
    
    system("rm ./Result/*");
    system("./Mono/runn.sh");
    system("cp ./Mono/MResult.txt ./Result/");
    system("rm ./Mono/*.txt");
    cout<<"Successfully Calibration"<<endl;
    return 1;
}

vector<double>  Fisheye_calibrate::FOV(Mat K, Size imageSize)
{
    double apertureWidth = 1;
    double apertureHeight = 1;
    double fieldOfViewX;
    double fieldOfViewY;
    double focalLength;
    cv::Point2d principalPoint;
    double aspectRatio;
    cv::calibrationMatrixValues(K, imageSize, apertureWidth, apertureHeight, fieldOfViewX, fieldOfViewY, focalLength, principalPoint, aspectRatio);
    
    aFOV.push_back(fieldOfViewX);
    aFOV.push_back(fieldOfViewY);
    return aFOV;
    
}

Fisheye_calibrate::~Fisheye_calibrate()
{
    std::cout << "Cature is freed" << std::endl;
}

