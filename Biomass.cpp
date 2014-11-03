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

Mat src;

//old code
//double getPlantWidth(Mat image){								//Retrieves green-filtered image to get the width of the lower half of the plant
//																//Can be tested within greenness function
//	int H;
//	int S;
//	int V;
//	int width;
//	int aveWidth = 0;
//	int upperBound = (image.rows / 3) * 2;
//	int lowerBound = image.rows-(image.rows/8);
//
//	//ofstream myfile;
//	//myfile.open("example.txt"); //FOR TESTING
//
//	for (int row = upperBound; row < lowerBound; ++row) {
//		width = 0;
//		for (int col = 0; col < image.cols; ++col) {
//			Vec3b pixel = image.at<cv::Vec3b>(row, col);
//			H = pixel[0]; // H
//			S = pixel[1]; // S
//			V = pixel[2]; // V
//			if (H != 0 && S != 0 && V != 255) {
//				width++;
//				//myfile << 1; //FOR TESTING
//
//			}
//			else {
//				//myfile << "0"; //FOR TESTING
//			}
//		}
//		//myfile << "\n"; //FOR TESTING
//		aveWidth += width;
//
//	}
//	aveWidth /= upperBound;
//	cout << "Average width: " << aveWidth << endl;
//	//myfile << aveWidth;
//	//myfile.close(); //FOR TESTING
//	return aveWidth;
//}
//end of old code

double getPlantWidth(Mat image){								

	int H = 0;
	int S = 0;
	int V = 225;
	int width;
	int colLeft;
	int colRight;
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

Mat segment(Mat im){
	Mat3b image = im.clone();
	Mat result;

	for (Mat3b::iterator it = image.begin(); it != image.end(); it++) {
		Vec3b rgbspace = *it;
		int B = rgbspace.val[0];
		int G = rgbspace.val[1];
		int R = rgbspace.val[2];

		if (!(G > R + 5 && G > B + 5))
			*it = Vec3b(255, 255, 255);
	}

	return image;
}

Mat noisefilter(Mat im){
	Mat3b image = im.clone();
	Mat3b image2;

	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2, 2),
		Point(1, 1));

	dilate(image, image2, element, Point(-1, -1), 2, 1, 1);
	erode(image2, image2, element, Point(-1, -1), 2, 1, 1);


	return image2;
}

Mat whiteBal(){
	Mat rgbim = src;

	if (rgbim.channels() >= 3)
	{
		Mat ycrcb;

		cvtColor(rgbim, ycrcb, CV_BGR2YCrCb);

		vector<Mat> channels;
		split(ycrcb, channels);

		equalizeHist(channels[0], channels[0]);

		Mat result;
		merge(channels, ycrcb);

		cvtColor(ycrcb, result, CV_YCrCb2BGR);

		//imshow("White Balance", result);
		return result;
	}
	return Mat();
}

int compareLCCvalues(int H, int S, int V){

	int val1 = abs(H - 36) + abs(S - 183) + abs(V - 167);
	int val2 = abs(H - 44) + abs(S - 157) + abs(V - 149);
	int val3 = abs(H - 46) + abs(S - 154) + abs(V - 121);
	int val4 = abs(H - 64) + abs(S - 101) + abs(V - 121);

	if (val1<val2 && val1<val3 && val1<val4){
		return 2;
	}
	else if (val2<val1 && val2<val3 && val2<val4){
		return 3;
	}
	else if (val3<val1 && val3<val2 && val3<val4){
		return 4;
	}
	else{
		return 5;
	}

}

Mat greenness(Mat im){
	Mat rgbim = im.clone();
	Mat3b image;
	cvtColor(rgbim, image, CV_BGR2HSV);

	int aveH = 0;
	int aveS = 0;
	int aveV = 0;
	int pixelct = 0;

	for (Mat3b::iterator it = image.begin(); it != image.end(); it++) {  //CONVERT THIS TO NESTED

		Vec3b hsv = *it;
		int H = hsv.val[0];
		int S = hsv.val[1];
		int V = hsv.val[2];

		if (H != 0 && S != 0 && V != 255){
			pixelct++;
			aveH += H;
			aveS += S;
			aveV += V;
		}
		else {
		}

	}

	int lccval = compareLCCvalues(aveH / pixelct, aveS / pixelct, aveV / pixelct);


	cvtColor(image, image, CV_HSV2BGR);

	char name[30];
	sprintf(name, "Average H=%d", aveH / pixelct);
	putText(image, name, Point(20, 40), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 0, 0), 2, 8, false);

	sprintf(name, "Average S=%d", aveS / pixelct);
	putText(image, name, Point(20, 80), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 0, 0), 2, 8, false);

	sprintf(name, "Average V=%d", aveV / pixelct);
	putText(image, name, Point(20, 120), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 0, 0), 2, 8, false);

	sprintf(name, "Pixels=%d", pixelct);
	putText(image, name, Point(20, 160), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 0, 0), 2, 8, false);

	sprintf(name, "LCC Value between %d and %d", lccval - 1, lccval);
	putText(image, name, Point(20, 200), FONT_HERSHEY_SIMPLEX, .7, Scalar(0, 0, 0), 2, 8, false);

	//imshow("Greenness", image);

	return image;

}

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