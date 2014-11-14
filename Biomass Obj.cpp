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

class Biomass{
    Mat src;
    
public:
    double getPlantWidth(Mat);
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
	int colLeft;
	int colRight = 0;
	int aveWidth = 0;
	int upperBound = (image.rows / 3) * 2;
	int lowerBound = image.rows ;

	//ofstream myfile;
	//myfile.open("example.txt"); //FOR TESTING

	for (int row = upperBound; row < lowerBound; ++row) {
		width = 0;
		colLeft = 0;
		for (int col = 0; col < image.cols; ++col) {
			Vec3b pixel = image.at<cv::Vec3b>(row, col);
			H = pixel[0]; // H
			S = pixel[1]; // S
			V = pixel[2]; // V
			if (H != 0 && S != 0 && V != 255) {
				colLeft = col;
				for (int col = image.cols - 1; col > 0; --col) {
					Vec3b pixel = image.at<cv::Vec3b>(row, col);
					H = pixel[0]; // H
					S = pixel[1]; // S
					V = pixel[2]; // V
					if (H != 0 && S != 0 && V != 255) {
						colRight = col;
						break;
					}
					else {
					}
				}
				width = colRight - colLeft;
				break;
			}
			else{
			}

		}			
		
		aveWidth += width;

	}
	aveWidth /= upperBound;
	//myfile << "ave width:" << aveWidth;
	//myfile.close(); //FOR TESTING
	return aveWidth;
}
/*
int main(){
	ofstream myfile;
	myfile.open("example.txt"); //FOR TESTING
	for (int i = 1; i < 61; i++){
		string x = "C:/Users/Micaela Angela/Dropbox/Butil - IRRI Project/Images/10-30-2014/IR64-0";
		if (i<10){
			x.append("0");
		}
		x.append(to_string(i));
		x.append(".JPG");
		src = imread(x, 1);
		if (src.empty()==false){
			Mat filtered = noisefilter(segment(whiteBal()));

			double temp = getPlantWidth(greenness(filtered));
			myfile << "image " << to_string(i) << ": " << temp << "\n";
			cout << temp << endl;
		}
		else{
		}	
	
	}
	myfile.close();
	//resize(src, src, Size(src.cols / 4, src.rows / 4));
	//imshow("Original", src);
	waitKey();
}
*/