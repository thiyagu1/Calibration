//
//  CameraCapture.cpp
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 11/04/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

#include "CameraCapture.hpp"

Capture::Capture()
{
    height = 0;
    width = 0;
    FPS = 0.0;
    std::cout << "Cature is initialized" << std::endl;
}

void Capture::Camprop(int x, int y, VideoCapture* cap)
{
    /*        Resolution
     
     2.2k - 15fps - 4416x1242
     1080p - 30fps - 3840x1080
     720p - 60fps - 2560x720
     WVGA - 100fps - 1344x376
     
     */
    
    height = x;
    width = y;
    //cap->set(CV_CAP_PROP_CONVERT_RGB, 0);
    //cap->set(CV_CAP_PROP_MODE, CV_CAP_MODE_YUYV);
    cap->set(CV_CAP_PROP_FOURCC ,CV_FOURCC('M', 'J', 'P', 'G') );
    cap->set(CV_CAP_PROP_FRAME_HEIGHT, height);
    cap->set(CV_CAP_PROP_FRAME_WIDTH, width);
    cap->set(CV_CAP_PROP_FPS, 120);
    
    
    cout <<"Dimention Camprop = "<<height<<"x"<<width<<endl;
    cout <<"fps = "<<cap->get(CV_CAP_PROP_FPS) <<endl;
    cout <<"FourCC = "<<cap->get(CV_CAP_PROP_FOURCC)<<endl;
    
    
}
void Capture::Hello()
{
    std::cout<<"I am in the Hello function"<< std::endl;
    
}

// This function will stream all the video frames from the USB cameras
void Capture::stream(int num)
{
    Mat shot, pic;
    cap = new VideoCapture(num);
    for(;;)
    {
        cap->read(shot);
        if( shot.empty() ) break; // end of video stream
        pic = shot.clone();
        imshow("Capture", shot);
        if( waitKey(2) == 113 ) break; // stop capturing by pressing q
    }
}

// This function will will take a snapshot of the video frames upto the number of count
void Capture::frameshot(int num, int count)
{
    Mat shot, pic;
    cap = new VideoCapture(num);
    for(int i=0;i<count;i++)
    {
        cap->read(shot);
        if( shot.empty() ) break; // end of video stream
        pic = shot.clone();
        imshow("Capture", shot);
        string savingName = "./Mono/" + std::to_string(i) + ".jpg";
        cv::imwrite(savingName, shot);
        if( waitKey(10) == 113 ) break; // stop capturing by pressing q
    }
}



// This function will will take a Buttonshot of the video frames upto the number of count
void Capture::Buttonshot(int num , int x, int y)
{
    Mat shot, left, right, pic;
    cap = new VideoCapture(num);
    char check;
    int add = 0;
    Camprop(x, y, cap);
    for(;;)
    {
        cap->read(shot);
        if( shot.empty() ) break; // end of video stream
        pic = shot.clone();
        // Extract left and right images from side-by-side
        left = shot(cv::Rect(0, 0, shot.cols / 2, shot.rows));
        right = shot(cv::Rect(shot.cols / 2, 0, shot.cols / 2, shot.rows));
        imshow("Capture", shot);
        check = cv::waitKey(30);
        if(check == 'c')
        {
            add++;
            string savingName1 = "./VLeftBig/" + std::to_string(add) + ".jpg";
            cv::imwrite(savingName1, left);
            string savingName2 = "/VRightBig/" + std::to_string(add) + ".jpg";
            cv::imwrite(savingName2, right);
            //if( waitKey(10) == 113 ) break; // stop capturing by pressing q
        }
        else if(check == 'q'){
            break;
        }
    }
}


// This function will will take a Buttonshot of the video frames upto the number of count
void Capture::Monoshot(int num , int x, int y)
{
    Mat shot, cam, pic;
    cap = new VideoCapture(num);
    char check;
    int add = 0;
    Camprop(x, y, cap);
    for(;;)
    {
        cap->read(shot);
        if( shot.empty() ) break; // end of video stream
        pic = shot.clone();
        // Extract left and right images from side-by-side
        imshow("Capture", shot);
        check = cv::waitKey(30);
        if(check == 'c')
        {
            add++;
            string savingName1 = "./VLeftBig/" + std::to_string(add) + ".jpg";
            cv::imwrite(savingName1, shot);
            
            //if( waitKey(10) == 113 ) break; // stop capturing by pressing q
        }
        else if(check == 'q'){
            break;
        }
    }

}

void Capture::stereoshot(int num1 ,int num2, int x, int y) // 480 , 640
{

    int row = y;
    int column = x;
    Mat shot, shot1, cam;
    cap = new VideoCapture(num1);
    cap1 = new VideoCapture(num2);
    char check;
    int add = 0;
    Camprop(row, column, cap);
    Camprop(row, column, cap1);
    for(;;)
    {
        int64 start = cv::getTickCount();
        cap->read(shot);
        cap1->read(shot1);
        
    //cout<<"Shot1 =" <<shot1.rows <<"x"<<shot1.cols<<endl;
    //cout<<"Shot =" <<shot.rows <<"x"<<shot.cols<<endl;

        row = shot.rows;
        column = shot1.cols + shot.cols;

        Mat res = Mat(row, column, CV_8UC3);
        cout<<"Res Merge (row,col) =" <<res.rows <<"x"<<res.cols<<endl;
        shot1.copyTo(res(Rect(0, 0, shot1.cols, shot1.rows)));
        shot.copyTo(res(Rect(shot1.cols, 0, shot.cols, shot.rows)));

        if( shot.empty() ) break; // end of video stream
        if( shot1.empty() ) break; // end of video stream
        
       //imshow("Capture", shot);
        //imshow("Capture1", shot1);
        imshow("Final", res);

        check = cv::waitKey(1);
        double fps = cv::getTickFrequency() / (cv::getTickCount() - start);
        std::cout << "FPS : " << fps << std::endl;
        
        if(check == 'c')
        {
            add++;
            string savingName1 = "./VLeftBig/" + std::to_string(add) + ".jpg";
            cv::imwrite(savingName1, shot);
            string savingName2 = "./VRightBig/" + std::to_string(add) + ".jpg";
            cv::imwrite(savingName2, shot1);

            if( waitKey(10) == 113 ) break; // stop capturing by pressing q
        }
        else if(check == 'q'){
            break;
        }
    }
}

Capture::~Capture (void)
{
    delete cap;
    std::cout << "Cature is freed" << std::endl;
}

