//
//  Rotation_YPR_convertion.cpp
//  Camera_Test
//
//  Created by Thiyagarajan Manihatty Bojan on 08/08/2018.
//  Copyright © 2018 Varjo. All rights reserved.
//

//      Input Rotation Matrix :
//
//           R = [0.9999936100852538, 0.003574842084593224, 1.710940292103228e-05;
//                -0.003574841824536402, 0.9999936101169358, -1.520616586632866e-05;
//                -1.716365323563225e-05, 1.514490529107057e-05, 0.9999999997380205]
//
//      Algorithm :
//
//           R = [(0,0) (0,1) (0,2)
//                (1,0) (1,1) (1,2)
//                (2,0) (2,1) (2,2)];
//
//           p = sqrt(((2,1)*(2,1)) + ((2,2)*(2,2)));
//           x = atan2((2,1), (2,2)); // Roll
//           y = atan2(-(1,0), p); // Pitch
//           Z = atan2((1,0),(0,0)) // Yaw
//
//      RESULT :
//           x = 1.51449e-05
//           y = 0.00357483
//           z = -0.00357485




#include "Rotation_YPR_convertion.hpp"

vector<double > R2YPR(Mat n){
    vector<double > Euler_Angles;
    double t,x,y,z;
    t=sqrt((n.at<double>(2,1)*n.at<double>(2,1)) + (n.at<double>(2,2)*n.at<double>(2,2)));
    cout <<t<<endl;
    x = atan2(n.at<double>(2,1), n.at<double>(2,2));
    y = atan2( -(n.at<double>(2,0)), t);
    z = atan2(n.at<double>(1,0), n.at<double>(0,0));
    Euler_Angles.push_back(x);
    Euler_Angles.push_back(y);
    Euler_Angles.push_back(z);
    return Euler_Angles;
}
