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
#include <math.h>
using namespace std;
using namespace cv;

//Config File
#define EXT ".jpg"
#define Units "mm"
#define View 0 // 1 - enable , 0 - disable
#define Option 1 // 0 - Left folder , 1 - Right folder , 2 - Both folders , Any other - Left Folder
#define convert 1000
#define Width 1008
#define Height 1008
#define ImageCount 91
#define SQsize 90//in millimeter(mm)
#define Board_H 6
#define Board_W 10
#define Main_Path "/Users/thiyaga/Documents/thiy/Codes/Camera_Test/build/Debug"
#define Left_Directory "./VLeftBig/"
#define Right_Directory "./VRightBig/"
#define Distortion_Image_No 8
#define Model 0 // Parm - 0 (Pinhole Model), Parm - 1 (Fisheye Model)
#endif /* VarjoVST_h */


