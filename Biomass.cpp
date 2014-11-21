//THSST1 Prototype using OpenCV and C++ for pre-processing and
//determining the greenness of a plant specimen

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
	double convertPixelToCm(double);
	Mat cropImage(Mat);
	void setSrc(Mat);
};

void Biomass::setSrc(Mat source){
    src=source;
}

double Biomass::getPlantWidth(Mat image){								

	int H = 0;
	int S = 0;
	int V = 225;
	int width;
	int aveWidth = 0;
		
	for (int i = 0; i < image.rows; ++i)
	{
			width = 0;
			uchar * pixel = image.ptr<uchar>(i);
			for (int j = 0; j < image.cols; ++j)
			{
				if (pixel[j] == 0)
				{
					width++;
				}
				else
				{
				}
			}
			aveWidth += width;
	}
	
	aveWidth /= image.rows;
	return aveWidth;
}

double Biomass::computePlantRadius(double diameter){
	return diameter*.5;
}

double Biomass::computePlantBiomass(double radius, double height){
	return M_PI*radius*radius*height;
}

double Biomass::convertPixelToCm(double width){
	return radius*10/157.6667;
}

