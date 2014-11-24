 //
//  Greenness module for determining greenness of plant
//  OpenCVDEmo
//
//  Created by Jan Villarosa on 11/3/14.
//  Copyright (c) 2014 Jan Villarosa. All rights reserved.
//

#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <stdio.h>
#include <stdlib.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

class Greenness{
    Mat src;
    Mat result;
    
public:
    float compareLCCValues(int,int,int);
    float greenness(Mat);
    Mat getResult();
};

Mat Greenness:: getResult(){
    return result;
}

float Greenness::compareLCCValues(int H, int S, int V){
    
    float val1 = abs(H-40.5);
    float val2 = abs(H-46);
    float val3 = abs(H-50.5);
    float val4 = abs(H-56.5);
    
    if (H<39.5){
        return 1.5;
    } else if(val1<val2 && val1<val3 && val1<val4){
        if(val1 < 1){
            return 2;
        }
        else{
            return 2.5;
        }
    }else if(val2<val1 && val2<val3 && val2<val4){
        if(val2 < 1){
            return 3;
        }
        else{
            return 3.5;
        }
    }else if(val3<val1 && val3<val2 && val3<val4){
        if(val2 < 1){
            return 4;
        }
        else{
            return 4.5;
        }
    }else{
        return 5;
    }
    
}

float Greenness::greenness(Mat im){
    Mat rgbim = im.clone();
    Mat3b image;
    
    cvtColor(rgbim,image,CV_BGR2HSV);
    
    int aveH = 0;
    int aveS = 0;
    int aveV = 0;
    int pixelct = 0;
    
    for (Mat3b::iterator it = image.begin(); it != image.end(); it++) {
        Vec3b hsv = *it;
        int H=hsv.val[0];
        int S=hsv.val[1];
        int V=hsv.val[2];
        
        if((H != 0 && S != 0 && V != 255)){
            pixelct++;
            aveH+=H;
            aveS+=S;
            aveV+=V;
        } else {
        }
    }
    float lccval = compareLCCValues(aveH/pixelct, aveS/pixelct, aveV/pixelct);
    lccval = floor(lccval * 1000.0) / 1000.0;
    
    
    cvtColor(image, image, CV_HSV2BGR);
    
    char name[30];
    sprintf(name,"Average H=%d",aveH/pixelct);
    putText(image,name, Point(40,80) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
    
    sprintf(name,"Average S=%d",aveS/pixelct);
    putText(image,name, Point(40,160) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
    
    sprintf(name,"Average V=%d",aveV/pixelct);
    putText(image,name, Point(40,240) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
    
    sprintf(name,"Pixels=%d",pixelct);
    putText(image,name, Point(40,320) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
    
    sprintf(name,"LCC Value: %.1f", lccval);
    putText(image,name, Point(40,400) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
    
    result = image;
    
    return lccval;
    
}