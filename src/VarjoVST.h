//
//  VarjoVST.h
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 11/04/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

#ifndef VarjoVST_h
#define VarjoVST_h
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/videoio.hpp>
#include <fstream>
#include <iterator>
#include <string>
#include <stdlib.h>
using namespace std;
using namespace cv;

//Config File
#define EXT ".jpg";
#define Width 1008;
#define Height 1008;
#define ImageCount 34;
#define SQsize 90;//in millimeter(mm)
#define Board_H 6;
#define Board_W 10;
#define Left_Directory "./VLeftBig/";
#define Right_Directory "./VRightBig/";
#define Distortion_Image_No 6;

#endif /* VarjoVST_h */

