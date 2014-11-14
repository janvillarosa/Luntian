//THSST1 Prototype using OpenCV and C++ for pre-processing and
//determining the greenness of a plant specimen

#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class WatershedSegmenter{
private:
    cv::Mat markers;
public:
    void setMarkers(cv::Mat& markerImage)
    {
        markerImage.convertTo(markers, CV_32S);
    }
    
    cv::Mat process(cv::Mat &image)
    {
        cv::watershed(image, markers);
        markers.convertTo(markers,CV_8U);
        return markers;
    }
};

class Preprocessor{
    Mat src;
    
    public:
        Mat segment(Mat);
        Mat noisefilter(Mat);
        Mat whiteBal();
        void setSrc(Mat);
};

void Preprocessor::setSrc(Mat source){
    src=source;
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
    
    imshow("Thresholded Image", imgThresholded);
    cvtColor(imgThresholded, imgThresholded, CV_GRAY2BGR);
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

Mat Preprocessor::noisefilter(Mat im){
    Mat3b image = im.clone();
    
    //opening (remove small objects from the foreground)
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    dilate( image, image, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    
    //closing (fill small holes in the foreground)
    dilate( image, image, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    erode(image, image, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );

    
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
        
        imshow("White Balance",result);
        return result;
    }
    return Mat();
}