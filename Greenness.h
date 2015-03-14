//
//  Greenness.h
//  OpenCVDEmo
//
//  Created by Jan Villarosa on 11/3/14.
//  Copyright (c) 2014 Jan Villarosa. All rights reserved.
//

#ifndef __GREENNESS_H_INCLUDED__ 
#define __GREENNESS_H_INCLUDED__ _

#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Greenness{
	Mat src;
	Mat result;

public:
	float compareLCCValues(int, int, int);
	float greenness(Mat);
	Mat getResult();
};
#endif /* defined(__OpenCVDEmo__Greenness__) */
