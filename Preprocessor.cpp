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
    
    return orig;
}

Mat Preprocessor::bin_segment(Mat wBal){
    Rect roi(600, 2300, 700, 300); //temporary placement
    Mat src_roi = wBal(roi);        //crops lower portion of the plant for further processing
    src_roi.copyTo(wBal);      // processed image will be cropped at the sides using cropImage()
    
    Mat src_gray;                               //temporary placement
    cvtColor(wBal, src_gray, CV_RGB2GRAY);  //of this code block
    threshold(src_gray, wBal, 170, 255, 0); //here. Code runs.
    
    //opening (remove small objects from the foreground)
    erode(wBal, wBal, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( wBal, wBal, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    //closing (fill small holes in the foreground)
    dilate( wBal, wBal, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(wBal, wBal, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    return wBal;
}

Mat Preprocessor::segment(Mat im){

    Mat3b image;
    Mat3b orig = im.clone();
    Mat result;
    
    int iLowH = 22;
    int iHighH = 75;
    
    int iLowS = 60;
    int iHighS = 255;
    
    int iLowV = 30;
    int iHighV = 255;
    
    cvtColor(im,image,CV_BGR2HSV);
    
    Mat imgThresholded;
    
    inRange(image, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
    
    //opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    //closing (fill small holes in the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    return imgThresholded;
}

Mat Preprocessor::noisefilter(Mat im){
    Mat3b image = im.clone();
    
    //opening (remove small objects from the foreground)
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    //closing (fill small holes in the foreground)
    dilate( image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
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
        
        Mat result;
        merge(channels,ycrcb);
        
        cvtColor(ycrcb,result,CV_YCrCb2BGR);
        
        return result;
    }
    return Mat();
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
    Rect roi(i, 0, k - i, 300);
    Mat image_roi = image(roi);
    image_roi.copyTo(image);
    //imshow("New Crop",image);

    return image;
}