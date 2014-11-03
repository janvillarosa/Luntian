//#include <iostream>
//#include <fstream>
//#define _USE_MATH_DEFINES
//#include <math.h>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//using namespace std;
//using namespace cv;
//
//class Biomass {
//public:
//	Mat filterPlant(String);
//	double getPlantWidth(Mat);
//	double computePlantRadius(double);
//	double computePlantBiomass(double, double);
//};
//
//double Biomass::getPlantWidth(Mat image){								//Retrieves green-filtered image to get the width of the lower half of the plant
//																//Can be tested within greenness function
//int H;
//int S;
//int V;
//int width;
//int aveWidth = 0;
//int bound = image.rows / 2;
//
///*ofstream myfile;
//myfile.open("example.txt"); //FOR TESTING*/
//
//for (int row = bound; row < image.rows; ++row) {
//	width = 0;
//	for (int col = 0; col < image.cols; ++col) {
//		Vec3b pixel = image.at<cv::Vec3b>(row, col);
//		H = pixel[0]; // H
//		S = pixel[1]; // S
//		V = pixel[2]; // V
//		if (H != 0 && S != 0 && V != 255) {
//			width++;
//			//myfile << width; //FOR TESTING
//
//		}
//		else {
//			//myfile << "0"; //FOR TESTING
//		}
//	}
//	//myfile << "\n"; //FOR TESTING
//	aveWidth += width;
//
//}
//aveWidth /= bound;
//std::cout << "Average width: " << aveWidth << std::endl;
////myfile.close(); //FOR TESTING
//return aveWidth;
//}
//
//Mat Biomass::filterPlant(String image){
//
//	int iLowH = 36;
//	int iHighH = 64;
//
//	int iLowS = 101;
//	int iHighS = 183;
//
//	int iLowV = 121;
//	int iHighV = 167;
//
//	Mat imgOriginal = imread(image, 1);
//	Mat imgHSV;
//	Mat imgThresholded;
//
//	while (true){
//	
//
//	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
//
//	
//
//	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
//
//	//morphological opening (remove small objects from the foreground)
//	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
//	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
//
//	//morphological closing (fill small holes in the foreground)
//	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
//	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
//
//	imshow("Thresholded Image", imgThresholded); //show the thresholded image
//
//	imshow("Original", imgOriginal);
//	imshow("HSV", imgHSV);
//
//	if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//	{
//		cout << "esc key is pressed by user" << endl;
//		break;
//	}
//	}
//
//	return imgThresholded;
//}
//
//double Biomass::computePlantRadius(double diameter){
//	return diameter*.5;
//};
//
//double Biomass::computePlantBiomass(double radius, double height){
//	return M_PI*radius*radius*height;
//};
//
//int main()
//{
//	Biomass biomass;
//	biomass.getPlantWidth(biomass.filterPlant("C:/image.jpg"));
//
//	cout << "Radius: " << biomass.computePlantRadius(10) << endl;
//	cout << "Biomass: " << biomass.computePlantBiomass(5, 4) << endl;
//	system("pause");
//}


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
};

double Biomass::getPlantWidth(Mat image){								//Retrieves green-filtered image to get the width of the lower half of the plant
																//Can be tested within greenness function
	int H;
	int S;
	int V;
	int width;
	int aveWidth = 0;
	int bound = image.rows / 2;

	/*ofstream myfile;
	myfile.open("example.txt"); //FOR TESTING*/

	for (int row = bound; row < image.rows; ++row) {
		width = 0;
		for (int col = 0; col < image.cols; ++col) {
			Vec3b pixel = image.at<cv::Vec3b>(row, col);
			H = pixel[0]; // H
			S = pixel[1]; // S
			V = pixel[2]; // V
			if (H != 0 && S != 0 && V != 255) {
				width++;
				//myfile << width; //FOR TESTING

			}
			else {
				//myfile << "0"; //FOR TESTING
			}
		}
		//myfile << "\n"; //FOR TESTING
		aveWidth += width;

	}
	aveWidth /= bound;
	std::cout << "Average width: " << aveWidth << std::endl;
	//myfile.close(); //FOR TESTING
	return aveWidth;
}