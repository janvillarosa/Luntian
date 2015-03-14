#ifndef __BIOMASS_H_INCLUDED__ 
#define __BIOMASS_H_INCLUDED__ 

#include <fstream>
#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

class Biomass {
	Mat src;
public:
	double getPlantWidth(Mat);
	double computePlantRadius(double);
	double computePlantBiomass(double, double);
	double computePixelToInchValue(int, double);
	double convertPixelToCm(double, int, double);
	void setSrc(Mat);
};

#endif