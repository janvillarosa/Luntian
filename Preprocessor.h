#ifndef __PREPROCESSOR_H_INCLUDED__ 
#define __PREPROCESSOR_H_INCLUDED__ 

#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Preprocessor{
	Mat src;
	int imgRow;
	int imgCol;

public:
	Mat segment(Mat);
	Mat noisefilter(Mat);
	Mat whiteBal();
	void setSrc(Mat);
	Mat cropImage(Mat);
	Mat bin_segment(Mat);
	Mat rgb_segment(Mat, Mat);
	Mat cropRGBImage(Mat);
};

#endif