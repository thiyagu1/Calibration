//
//  CameraCapture.hpp
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 11/04/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

#ifndef CameraCapture_hpp
#define CameraCapture_hpp
#include "VarjoVST.h"
namespace VST {

    class Capture {
    private:
        int height;
        int width;
        double FPS;
        VideoCapture* cap;
        VideoCapture* cap1;
    public:
        Capture ();
        void Camprop(int x, int y, VideoCapture* num);
        void Hello ();
        void stream(int num);
        void frameshot(int num, int count);
        void Buttonshot(int num , int x, int y);
        void Monoshot(int num , int x, int y);
        void stereoshot(int , int , int , int );
        ~Capture ();
    };
}
using namespace VST;

#endif /* CameraCapture_hpp */
