//THSST1 Prototype using OpenCV and C++ for pre-processing and
//determining the greenness of a plant specimen

#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Preprocessor{
    Mat src;
    
    public:
        Mat segment(Mat);
        Mat noisefilter(Mat);
        Mat whiteBal();
        void setSrc(Mat);
        Mat cropImage(Mat);
        Mat bin_segment(Mat);
        Mat rgb_segment(Mat, Mat);
};

void Preprocessor::setSrc(Mat source){
    src=source;
}

Mat Preprocessor::rgb_segment(Mat imgThreshold, Mat im){
    Mat imgThresholded;
    Mat3b orig = im.clone();
    
    cvtColor(imgThreshold, imgThresholded, CV_GRAY2BGR);
    bitwise_and(imgThresholded, orig, orig);
    
    for (Mat3b::iterator it = orig.begin(); it != orig.end(); it++) {
        Vec3b hsv = *it;
        int B=hsv.val[0];
        int G=hsv.val[1];
        int R=hsv.val[2];
        
        if((B == 0 && G == 0 && R == 0)){
            *it = Vec3b(255,255,255);
        } else {
        }
    }
    
    //Rect roi(0, 0, orig.cols, orig.rows/4 + orig.rows/4 + orig.rows/4);
    //Mat image_roi = orig(roi);
    //image_roi.copyTo(orig);
    return orig;
}

Mat Preprocessor::bin_segment(Mat wBal){
	Rect roi(wBal.cols / 4, (wBal.rows / 4) * 3, wBal.cols / 3, wBal.rows / 10); //Updated but still for improvement
    Mat src_roi = wBal(roi);        
    src_roi.copyTo(wBal);      
    
    Mat src_gray;                               
    cvtColor(wBal, src_gray, CV_RGB2GRAY);  
    threshold(src_gray, wBal, 170, 255, 0);
    
    //opening (remove small objects from the foreground)
    erode(wBal, wBal, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );
    dilate( wBal, wBal, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );
    
    //closing (fill small holes in the foreground)
    dilate( wBal, wBal, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );
    erode(wBal, wBal, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)) );
    
    return wBal;
}

Mat Preprocessor::segment(Mat im){

    Mat3b image;
    Mat3b orig = im.clone();
    Mat result;
    
    int iLowH = 22;
    int iHighH = 75;
    
    int iLowS = 30;
    int iHighS = 255;
    
    int iLowV = 30;
    int iHighV = 255;
    
    cvtColor(im,image,CV_BGR2HSV);
    
    Mat imgThresholded;
    
    inRange(image, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
    
    return imgThresholded;
}

Mat Preprocessor::noisefilter(Mat im){
    Mat3b image = im.clone();
    
    //opening (remove small objects from the foreground)
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
    dilate( image, image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
    
    //closing (fill small holes in the foreground)
    dilate( image, image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
    
    return image;
}

Mat Preprocessor::whiteBal(){
    Mat rgbim = src;
    
    if(rgbim.channels() >= 3)
    {
        Mat ycrcb;
        
        cvtColor(rgbim,ycrcb,CV_BGR2YCrCb);
        
        vector<Mat> channels;
        split(ycrcb,channels);
        
        equalizeHist(channels[0], channels[0]);
        (channels[0], channels[0], 0, 255, NORM_MINMAX);
        
        Mat result;
        merge(channels,ycrcb);
        
        cvtColor(ycrcb,result,CV_YCrCb2BGR);
        
        //imshow("White Balance",result);
        return result;
    }
    return Mat();
    
//    Mat rgbim = src;
//    //return src;
//    
//    if(rgbim.channels() >= 3)
//    {
//        Mat hsv;
//        
//        cvtColor(rgbim,hsv,CV_BGR2HSV);
//        
//        vector<Mat> channels;
//        split(hsv,channels);
//        
//        //normalize(channels[0], channels[0], 0, 255, NORM_MINMAX);
//        //normalize(channels[1], channels[1], 0, 255, NORM_MINMAX);
//        normalize(channels[2], channels[2], 100, 255, NORM_MINMAX);
//        //equalizeHist(channels[1], channels[1]);
//        //equalizeHist(channels[2], channels[2]);
//        
//        Mat result;
//        merge(channels,hsv);
//        
//        cvtColor(hsv,result,CV_HSV2BGR);
//        
//        return result;
//    }
//    return Mat();
}

Mat Preprocessor::cropImage(Mat image){
    int leftBound = 0;
    int rightBound = 0;
    int threshold = 250;
    int i = 0;
    int k = 0;

    while (leftBound <= threshold && i<image.cols - 1){
        leftBound = 0;
        for (int j = 0; j < image.rows; ++j)
        {
            if (image.at<uchar>(j, i) != 255)
            {
                leftBound++;

            }
            else
            {
            }
        }

        i++;
    }

    k = image.cols - 1;

    while (rightBound <= threshold && k >= 0){
        rightBound = 0;

        for (int j = 0; j < image.rows; ++j)
        {
            if (image.at<uchar>(j, k) != 255)
            {
                rightBound++;
            }
            else
            {
            }
        }
        k--;
    }
    
	Rect roi(i, 0, k - i, image.rows); //Updated for scalability
    Mat image_roi = image(roi);
    image_roi.copyTo(image);
    //imshow("New Crop",image);

    return image;
}