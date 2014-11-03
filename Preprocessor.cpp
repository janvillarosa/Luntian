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
};

void Preprocessor::setSrc(Mat source){
    src=source;
}

Mat Preprocessor::segment(Mat im){
    Mat3b image=im.clone();
    Mat result;
    
    for (Mat3b::iterator it = image.begin(); it != image.end(); it++) {
        Vec3b rgbspace = *it;
        int B=rgbspace.val[0];
        int G=rgbspace.val[1];
        int R=rgbspace.val[2];
        
        if(!(G > R+1 && G > B+1))
            *it = Vec3b(255,255,255);
    }
    
    return image;
}

Mat Preprocessor::noisefilter(Mat im){
    Mat3b image = im.clone();
    Mat3b image2;
    
    Mat element = getStructuringElement( MORPH_ELLIPSE,
                                        Size( 2 , 2 ),
                                        Point( 1, 1 ) );
    
    dilate(image,image2,element, Point(-1, -1), 2, 1, 1);
    erode(image2,image2,element, Point(-1, -1), 2, 1, 1);

    
    return image2;
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