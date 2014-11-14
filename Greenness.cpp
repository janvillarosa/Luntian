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
    
public:
    int compareLCCValues(int,int,int);
    void greenness(Mat);
};

int Greenness::compareLCCValues(int H, int S, int V){
    
    int val1 = abs(H-36)+abs(S-183)+abs(V-167);
    int val2 = abs(H-44)+abs(S-157)+abs(V-149);
    int val3 = abs(H-46)+abs(S-154)+abs(V-121);
    int val4 = abs(H-64)+abs(S-101)+abs(V-121);
    
    if(val1<val2 && val1<val3 && val1<val4){
        return 2;
    }else if(val2<val1 && val2<val3 && val2<val4){
        return 3;
    }else if(val3<val1 && val3<val2 && val3<val4){
        return 4;
    }else{
        return 5;
    }
    
}

void Greenness::greenness(Mat im){
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
    int lccval = compareLCCValues(aveH/pixelct, aveS/pixelct, aveV/pixelct);
    
    
    cvtColor(image, image, CV_HSV2BGR);
    
    char name[30];
    sprintf(name,"Average H=%d",aveH/pixelct);
    putText(image,name, Point(20,40) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,0,0), 2,8,false );
    
    sprintf(name,"Average S=%d",aveS/pixelct);
    putText(image,name, Point(20,80) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,0,0), 2,8,false );
    
    sprintf(name,"Average V=%d",aveV/pixelct);
    putText(image,name, Point(20,120) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,0,0), 2,8,false );
    
    sprintf(name,"Pixels=%d",pixelct);
    putText(image,name, Point(20,160) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,0,0), 2,8,false );
    
    sprintf(name,"LCC Value between %d and %d",lccval-1,lccval);
    putText(image,name, Point(20,200) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,0,0), 2,8,false );
    
    imwrite("/Users/janvillarosa/Documents/Luntian/Greenness/IR64-055.JPG", image);
    imshow( "Greenness", image );
    
}