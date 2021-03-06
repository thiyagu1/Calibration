#include "CameraCapture.hpp"
#include "Mono__Calibration.hpp"
#include "Stereo__Calibration.hpp"
#include "Fisheye_calib.hpp"
#include "Rotation_YPR_convertion.hpp"


template <typename T>
bool write(std::string fname, T d)
{
    std::ofstream file(fname, std::ios::binary);
    if (file.good()) {
        file.write((const char*)&d, sizeof(T));
        return true;
    } else
        return false;
}


void rectifiedimage(char const* imgs_directory,int number, Mat mapx, Mat mapy, int pos);


    
//int SDump_result(Mat cam1.K, Mat cam1.D, Mat cam2.K, Mat cam2.D, Mat R, vec3d T, Mat E, Mat F );

int main(int argc, const char * argv[]) {
   
    //*******************************************************
    // To do camera capture
    //*******************************************************
    
    int choice;
    vector<double > Angles;
    double W = Width;
    double H = Height;
    int board_width, board_height, num_imgs;
    float square_size;
    char const* imgs_directory1;
    char const* imgs_directory2;
    string Filename;
    board_width = Board_W;
    board_height = Board_H;
    num_imgs = ImageCount;
    square_size = SQsize;
   
    if (Option == 0)
    {
        imgs_directory1 = Left_Directory;
    }
    else if(Option == 1)
    {
        imgs_directory1 = Right_Directory;
    }
    else if (Option == 2)
    {
        imgs_directory1 = Left_Directory;
        imgs_directory2 = Right_Directory;
    }
    else
    {
        cout<<"Please choose the correct Option in the Configuration"<<endl;
    }

    
    cout<<"**************************************************"<<endl;
    cout<< "Welcome to the Calibration Application"<<endl;
    cout<<"**************************************************"<<endl;
    cout<<endl<<endl;
    cout<<"Menus : "<<endl;
    cout<<"0. Configuration Settings: "<<endl<<"1. Camera Capture CheckerBoard"<<endl<<"2. Single Camera Calibration"<<endl<<"3. Rotation matrix"<<endl<<"4.Stereo Camera Calibration"<<endl;
    cout<<endl<<"Please enter your choice : "<<endl;
    cin>>choice;
    cout<<"So your choice is = "<<choice<<endl;
    cout<<"**************************************************"<<endl;
 
    bool out = true;
    
 while(out)
 {
     int z;
     switch (choice) {
        case 0:
            {
                
            cout<< "Welcome to the Configuration Settings"<<endl;
            cout<< "We would need to know the following details :"<<endl;
            cout<< "a. Checker Board_width"<<endl<<"b. Checker Board Height"<<endl<<"c. Number of images" <<endl<<"d. Square Size of checker board (in mm)"<<endl<<endl<<"Note : Images needed to be stored in Leftcam and Rightcam folders located in the current folder"<<endl;
            cout<<"a. Checker Board_width"<<endl;
            cin>>board_width;
            cout<<"b. Checker Board Height"<<endl;
            cin>>board_height;
            cout<<"c. Number of images" <<endl;
            cin>>num_imgs;
            cout<<"d. Square Size of checker board (in mm)"<<endl;
            cin>>square_size;
                cin >> z;
            }  if(z == 1)
            {
                choice=2;
            }
            else if(z==2)
                choice=3;
             else
                 out = false;
                 break;
            
        case 1:
            {
            int k, x1, y1, dev1, dev2;
            cout<< "Welcome to the Capture Settings"<<endl;
            cout<< "We would need to know the following details :"<<endl;
                cout<<"Press any of the following:"<<endl<<" 1.Single Camera Calibration" <<endl<<" 2.Stereo Camera Calibration"<<endl;
            cin>>k;
            if(k == 1)
            {
                cout<<"You are interested in Single Camera Calibration :"<<endl;
                cout<<"Set the Camera Resolution :"<<endl<<" a. Weight(X):"<<endl;
                cin>>x1;
                cout<<"b.Height (Y) :"<<endl;
                cin>>y1;
                cout<<"Change cameras [0] - Webcam, [1] - USB camera, [2] - other USB camers"<<endl;
                cin>>dev1;
                Capture a;
                Mat img;
                Capture *frame;
                frame = &a;
                cout<<"Capturing Started :"<<endl;
                frame->Monoshot(dev1, y1, x1);
                cout<<"Capturing Completed :"<<endl;
                cout<<"Images are stored in the current working directory"<<endl;
            } else if(k == 2){
                
                cout<<"You are interested in Stereo Camera Calibration :"<<endl;
                cout<<"Set the Camera Resolution :"<<endl<<" a. Weight(X):"<<endl;
                cin>>x1;
                cout<<"b.Height (Y) :"<<endl;
                cin>>y1;
                cout<<"Change cameras [0] - Webcam, [1] - USB camera, [2] - other USB camers"<<endl;
                cin>>dev1;
                cout<<"Also another camera"<<endl;
                cin>>dev2;
                Capture a;
                Mat img;
                Capture *frame;
                frame = &a;
                cout<<"Capturing Started :"<<endl;
                frame->stereoshot(dev1, dev2, y1, x1);
                cout<<"Capturing Completed :"<<endl;
                cout<<"Images are stored in the current working directory"<<endl;
            }
            else{
                cout<<"incorrect value"<<endl;
                
                // Stream a USB camera
                //frame->stream(1);
                
                // Take a pic from the USB camera
                //frame->frameshot(0, 20);
                
                
                // Take a pic from the USB camera
                //frame->Buttonshot(1, 1242, 4416);
                
                // Take a Monoshot from the USB camera
                //frame->Monoshot(0, 480, 640);
                
                //    frame->stereoshot(0, 2, 480, 640); //89.65 or 90
                //

            }
            }
             out= false;
            break;
            
        case 2:
            {
            int m = Model;
                cout<<"Your selected Folder : "<<imgs_directory1<<endl;
            if( m == 0)
            {
            cout<< "Welcome to the Single Camera Calibration - Pinhole Model"<<endl;
            Mono_calibrate Mono(board_width, board_height, num_imgs,square_size, imgs_directory1);
            Mono.Varjo_initialize();
            }
            else
            {
                cout<< "Welcome to the Single Camera Calibration - Fish Eye Model"<<endl;
                Fisheye_calibrate Fish(board_width, board_height, num_imgs,square_size, imgs_directory1);
                Fish.Varjo_initialize();
            }
            }
             out= false;
            break;
            
         case 3:
         {
             double matrix[] ={0.9999936100852538, 0.003574842084593224, 1.710940292103228e-05,
                 -0.003574841824536402, 0.9999936101169358, -1.520616586632866e-05,
                 -1.716365323563225e-05, 1.514490529107057e-05, 0.9999999997380205};
             
             Mat R_Mat(3,3, CV_64F,matrix);
             //cout<<R_Mat<<endl;
                 cout<< "Conversion Rotation Matrix to YAW, Pitch and Roll"<<endl;
             
             //Angles = R2YPR(R_Mat);
             Angles = R2YPR(R_Mat);
             cout<<"x = "<<Angles[0]<<endl;
             cout<<"y = "<<Angles[1]<<endl;
             cout<<"z = "<<Angles[2]<<endl;
             }
             
             out= false;
             break;
        
        case 4:
            {
            cout<< "Welcome to the Stereo Camera Calibration"<<endl;
            cout<< "By default its 10 x 6 Checker Board Calibration "<<endl;
            Mat R; // R stores the rotation from Left to Right
            Mat F, E; //    Fundamental Matrix, Essential Matrix
            Vec3d T; // T stores the translation from Left to Right
            Mat R1, R2; // Rectification Transform for Left Camera and Right Camera.
            Mat P1, P2; // Projection Matrix in new rectified coordinate system for Left and Right Cameras.
            Mat Q; // Disparity to Depth Mapping
                if (Option == 2){
                    Stereo_calibrate cam1(board_width, board_height, num_imgs,square_size, imgs_directory1);
                    Stereo_calibrate cam2(board_width, board_height, num_imgs,square_size, imgs_directory2);
            cam1.Varjo_initialize();
            cam2.Varjo_initialize();
//            cam1.flag |= CV_CALIB_FIX_INTRINSIC;
//            cam2.flag |= CV_CALIB_FIX_INTRINSIC;
                int flag = 0;
                 flag |= CV_CALIB_FIX_INTRINSIC;
                
//                double rms = stereoCalibrate(cam1.RealP, cam1.ImageP, cam2.ImageP, cam1.K, cam1.D, cam2.K, cam2.D, cam1.img.size(), R, T, E,F, CALIB_FIX_ASPECT_RATIO +
//                                             CALIB_ZERO_TANGENT_DIST +
//                                             CALIB_USE_INTRINSIC_GUESS +
//                                             CALIB_SAME_FOCAL_LENGTH +
//                                             CALIB_RATIONAL_MODEL +
//                                             CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5, TermCriteria(TermCriteria::COUNT|TermCriteria::EPS,30,1e-6));
//
//
//
                
                double rms = stereoCalibrate(cam1.RealP, cam1.ImageP, cam2.ImageP, cam1.K, cam1.D, cam2.K, cam2.D, cam1.img.size(), R, T, E,F,CV_CALIB_SAME_FOCAL_LENGTH, TermCriteria(TermCriteria::COUNT|TermCriteria::EPS,30,1e-6));
       
                
                Angles = R2YPR(R);
                cout<<"x_1 = "<<Angles[0]<<endl;
                cout<<"y_2 = "<<Angles[1]<<endl;
                cout<<"z_3 = "<<Angles[2]<<endl;
                cout<<"x = "<<T[0]<<endl;
                cout<<"y = "<<T[1]<<endl;
                cout<<"z = "<<T[2]<<endl;
                cout<<"x = "<<-(T[0]/1000)<<endl;
                cout<<"y = "<<-(T[1]/1000)<<endl;
                cout<<"z = "<<-(T[2]/1000)<<endl;
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
                    
                } intr1, intr2;
                
                struct Extrinsics {
                    float roll;
                    float pitch;
                    float yaw;
                    float translationX;
                    float translationY;
                    float translationZ;
                }extr1,extr2;
                
                extr1.roll = 0.0; // x
                extr1.pitch = 0.0; // y
                extr1.yaw = 0.0; // z
                extr1.translationX = 0.0;
                extr1.translationY = 0.0;
                extr1.translationZ = 0.0;
                
                extr2.roll = Angles[0];
                extr2.pitch = Angles[1];//+0.065752;
                extr2.yaw = Angles[2];
                extr2.translationX = -(T[0]/1000);
                extr2.translationY = -(T[1]/1000);
                extr2.translationZ = -(T[2]/1000);
                write("./Result/extrinsicsLeft.bin", extr1);
                write("./Result/extrinsicsRight.bin", extr2);
                
                intr1.K1 = cam1.D.at<double>(0,0);
                intr1.K2 = cam1.D.at<double>(0,1);
                intr1.K3 = cam1.D.at<double>(0,4);
                intr1.K4 = 0.0;
                intr1.P1 = cam1.D.at<double>(0,2);
                intr1.P2 = cam1.D.at<double>(0,3);
                intr1.Fx = cam1.K.at<double>(0,0)/W;
                intr1.Fy = cam1.K.at<double>(1,1)/H;
                intr1.Cx = cam1.K.at<double>(0,2)/W;
                intr1.Cy = cam1.K.at<double>(1,2)/H;
                
                intr1.aspectRatio = (double) W/H;
                cout<<"Aspect Ratio = "<<intr1.aspectRatio<<endl;
                
//                std::ofstream outFile1("intrinsics1.raw", std::ios::binary);
//                outFile1.write((const char*)&intr1, sizeof(intrinsics));
//
                
                intr2.K1 = cam2.D.at<double>(0,0);
                intr2.K2 = cam2.D.at<double>(0,1);
                intr2.K3 = cam2.D.at<double>(0,4);
                intr2.K4 = 0.0;
                intr2.P1 = cam2.D.at<double>(0,2);
                intr2.P2 = cam2.D.at<double>(0,3);
                intr2.Fx = cam2.K.at<double>(0,0)/W;
                intr2.Fy = cam2.K.at<double>(1,1)/H;
                intr2.Cx = cam2.K.at<double>(0,2)/W;
                intr2.Cy = cam2.K.at<double>(1,2)/H;
                
                intr2.aspectRatio = (double) W/H;
                cout<<"Aspect Ratio = "<<intr2.aspectRatio<<endl;
                write("./Result/intrinsicsLeft.bin", intr1);
                write("./Result/intrinsicsRight.bin", intr2);
//                std::ofstream outFile2("intrinsics2.raw", std::ios::binary);
//                outFile2.write((const char*)&intr2, sizeof(intrinsics));
//                
                
                
             //   Rect validRoI[2];
                Mat map1, map2, map3, map4;
                int number = Distortion_Image_No;
                
           // stereoRectify(cam1.K, cam1.D, cam2.K, cam2.D, cam1.img.size(), R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY,1,cam1.img.size(), &validRoI[0], &validRoI[1]);
            
                stereoRectify(cam1.K, cam1.D, cam2.K, cam2.D, cam1.img.size(), R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY);
               
                
                cout <<"good"<<endl;
                initUndistortRectifyMap(cam1.K, cam1.D, R1, P1, cam1.img.size(), CV_32FC1, map1, map2);
                initUndistortRectifyMap(cam2.K, cam2.D, R2, P2, cam2.img.size(), CV_32FC1, map3, map4);
                rectifiedimage(imgs_directory1, number, map1,map2, 1);
                rectifiedimage(imgs_directory2, number, map3,map4, 2);
              

              
                
            cout <<endl<<"Generating Result : "<<endl;
                {
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Camera Matrix : "<<endl;
                    cout <<cam1.K<<endl;
                    std::ofstream output_file1("./Stereo/B1_Camera_Matrix.txt");
                    output_file1 << cam1.K;
                    output_file1.close();
                    cout<<"*****************************************"<<endl;
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Camera Matrix : "<<endl;
                    cout <<cam2.K<<endl;
                    std::ofstream output_file2("./Stereo/B2_Camera_Matrix.txt");
                    output_file2 << cam2.K;
                    output_file2.close();
                    cout<<"*****************************************"<<endl;
                 
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Distortion Coefficient : "<<endl;
                    cout<<"K1,K2,P1,P2,K3"<<endl;
                    cout <<cam1.D<<endl;
                    std::ofstream output_file3("./Stereo/B1_Distortion_Coefficient.txt");
                    output_file3 << cam1.D;
                    output_file3.close();
                    cout<<"*****************************************"<<endl;
                 
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Distortion Coefficient : "<<endl;
                    cout<<"K1,K2,P1,P2,K3"<<endl;
                    cout <<cam2.D<<endl;
                    std::ofstream output_file4("./Stereo/B2_Distortion_Coefficient.txt");
                    output_file4 << cam2.D;
                    output_file4.close();
                    cout<<"*****************************************"<<endl;
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Rotation Matrix: "<<endl;
                    cout <<R<<endl;
                    
                    std::ofstream output_file5("./Stereo/B_Rotation_vector.txt");
                    output_file5 << R;
                    output_file5.close();
                    cout<<"*****************************************"<<endl;
                    
                    
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Translation Vector : "<<endl;
                    cout <<T<<endl;
                    std::ofstream output_file6("./Stereo/B_Translation_vector.txt");
                    output_file6 << T;
                    output_file6.close();
                    cout<<"*****************************************"<<endl;
                    
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Essential_Matrix : "<<endl;
                    cout <<E<<endl;
                    std::ofstream output_file7("./Stereo/B_Essential_Matrix.txt");
                    output_file7 << E;
                    output_file7.close();
                    cout<<"*****************************************"<<endl;
                    
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Fundamental Matrix : "<<endl;
                    cout <<F<<endl;
                    std::ofstream output_file8("./Stereo/B_Fundamental_Matrix.txt");
                    output_file8 << F;
                    output_file8.close();
                    cout<<"*****************************************"<<endl;
                    
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Rectification Transform Left: "<<endl;
                    cout <<R1<<endl;
                    std::ofstream output_file9("./Stereo/B1_Rectification_Transform.txt");
                    output_file9 << R1;
                    output_file9.close();
                    cout<<"*****************************************"<<endl;
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Rectification Transform Right: "<<endl;
                    cout <<R2<<endl;
                    std::ofstream output_file10("./Stereo/B2_Rectification_Transform.txt");
                    output_file10 << R2;
                    output_file10.close();
                    cout<<"*****************************************"<<endl;
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Projection Matrix Left: "<<endl;
                    cout <<P1<<endl;
                    std::ofstream output_file11("./Stereo/B1_Projection_Matrix.txt");
                    output_file11 << P1;
                    output_file11.close();
                    cout<<"*****************************************"<<endl;
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Projection Matrix Right: "<<endl;
                    cout <<P2<<endl;
                    std::ofstream output_file12("./Stereo/B2_Projection_Matrix.txt");
                    output_file12 << P2;
                    output_file12.close();
                    cout<<"*****************************************"<<endl;
                    
                    cout<<endl;
                    cout<<"*****************************************"<<endl;
                    cout<<"Disparity Map : "<<endl;
                    cout <<Q<<endl<<endl;
                    cout <<"RMS = "<<rms<<endl;
                    std::ofstream output_file13("./Stereo/B_Disparity_Map.txt");
                    output_file13 << Q;
                    output_file13.close();
                    cout<<"*****************************************"<<endl;
                   // system("rm ./Result/*");
                    system("./Stereo/runn.sh");
                    system("cp ./Stereo/RESULT.txt ./Result/");
                    system("rm ./Stereo/*.txt");
                    }
                    }
                else
                    cout<<"Please choose the correct Option in the Configuration File"<<endl;
                }
                    out= false;
                    break;
            
        default: cout<<"Enter a valid number"<<endl;
             out= false;
            break;
    }
    
 }
    
    
   }

void rectifiedimage(char const* imgs_directory,int number, Mat mapx, Mat mapy, int pos)
{
    Mat img, imgk,ori, imgdis_corrected;
    string Filename = imgs_directory + std::to_string(number) + EXT;
    ori = imread(Filename, CV_LOAD_IMAGE_COLOR);
    cout<<Filename<<endl;
    imgk = imread(Filename, CV_LOAD_IMAGE_COLOR);
    //undistort(imgk, imgdis_corrected, camera_matrix, Distortion_coeff);
    remap(imgk, imgdis_corrected, mapx, mapy, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
    cout<<"Remap done"<<endl;
    string Save_corrected ="./Result/Distortion_Corrected" + std::to_string(pos) + EXT;
    imwrite( Save_corrected, imgdis_corrected );
    string Save_original ="./Result/Undistorted_Original" + std::to_string(pos) + EXT;
    imwrite( Save_original, ori );
}


