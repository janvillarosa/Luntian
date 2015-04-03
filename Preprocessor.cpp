//THSST Prototype using OpenCV and C++ for pre-processing and
//determining the greenness of a plant specimen

#include "Preprocessor.h"

void Preprocessor::setSrc(Mat source){
    src=source;
}

Mat Preprocessor::rgb_segment(Mat imgThreshold, Mat im){
    Mat imgThresholded;
    Mat3b orig = im.clone();
    
    cvtColor(imgThreshold, imgThresholded, CV_GRAY2BGR);
    //bitwise_not(imgThresholded, imgThreshold);
    bitwise_or(imgThresholded, orig, orig);
    
    
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
    
    for (Mat3b::iterator it = orig.begin(); it != orig.end(); it++) {
        Vec3b hsv = *it;
        int B=hsv.val[0];
        int G=hsv.val[1];
        int R=hsv.val[2];
        
        if((B >= 80 && R >= 80)){
           *it = Vec3b(255,255,255);
        } else {
        }
    }
    return orig;
}

Mat Preprocessor::bin_segment(Mat wBal){
	Rect roi(wBal.cols / 4, (wBal.rows / 5) * 4, wBal.cols / 2, wBal.rows / 14); //Updated but still for improvement
    Mat src_roi = wBal(roi);
    src_roi.copyTo(wBal);
    
    Mat src_gray;                               
    cvtColor(wBal, src_gray, CV_RGB2GRAY);  
    threshold(src_gray, wBal, 170, 255, THRESH_OTSU);
    
    return wBal;
}

Mat Preprocessor::segment(Mat im){
    
    Mat imgThresholded;
    Mat image = im.clone();
    
    cvtColor(image,image,CV_RGB2GRAY);
    
//    int iLowH = 50;
//    int iHighH = 90;
//    
//    int iLowS = 0;
//    int iHighS = 255;
//    
//    int iLowV = 0;
//    int iHighV = 255;
//    
//    
//    inRange(image, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
//    bitwise_not(imgThresholded, imgThresholded);
    
    threshold(image, imgThresholded, 190, 255, THRESH_OTSU);
    
    
    //opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    
    //closing (fill small holes in the foreground)
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(2, 2)) );
    
    return imgThresholded;
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
    
    Mat image = src;
    Mat rgbim;
    
    Rect roi(0, 0, image.cols, image.rows-65); //Updated for scalability
    Mat image_roi = image(roi);
    image_roi.copyTo(rgbim);
    
    if(rgbim.channels() >= 3)
    {
        Mat hsv;
        
        cvtColor(rgbim,hsv,CV_BGR2HSV);
        vector<Mat> channels;
        split(hsv,channels);
        
        //normalize(channels[0], channels[0], 0, 100, NORM_MINMAX);
        normalize(channels[1], channels[1], 10, 255, NORM_MINMAX);
        normalize(channels[2], channels[2], 20, 255, NORM_MINMAX);
        
        Mat result;
        merge(channels,hsv);
        
        cvtColor(hsv,result,CV_HSV2BGR);
        
        return result;
    }
    return Mat();
}

Mat Preprocessor::cropRGBImage(Mat image){
    
    int i = 0;
    bool notFound = true;
    
    while (notFound && i<image.rows - 1){
        for (int j = 0; j < image.cols; ++j)
        {
            if (image.at<uchar>(i, j) != 255)
            {
                notFound = false;
                
            }
            else
            {
            }
        }
        
        i++;
    }
    imgCol=i;
    imgRow=(image.rows-i)/4 + (image.rows-i)/4 + (image.rows-i)/4;
    
    Rect roi(0, imgCol, image.cols, imgRow); //Updated for scalability
    Mat image_roi = image(roi);
    image_roi.copyTo(image);
    //imshow("New Crop",image);
    
    return image;
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