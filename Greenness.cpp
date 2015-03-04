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
    float compareLCCValues(int);
    float greenness(Mat);
    Mat getResult();
};

Mat Greenness:: getResult(){
    return result;
}

Scalar HSVtoRGBcvScalar(int H, int S, int V) {
    
    int bH = H; // H component
    int bS = S; // S component
    int bV = V; // V component
    float fH, fS, fV;
    float fR, fG, fB;
    const float FLOAT_TO_BYTE = 255.0f;
    const float BYTE_TO_FLOAT = 1.0f / FLOAT_TO_BYTE;
    
    // Convert from 8-bit integers to floats
    fH = (float)bH * BYTE_TO_FLOAT;
    fS = (float)bS * BYTE_TO_FLOAT;
    fV = (float)bV * BYTE_TO_FLOAT;
    
    // Convert from HSV to RGB, using float ranges 0.0 to 1.0
    int iI;
    float fI, fF, p, q, t;
    
    if( bS == 0 ) {
        // achromatic (grey)
        fR = fG = fB = fV;
    }
    else {
        // If Hue == 1.0, then wrap it around the circle to 0.0
        if (fH >= 1.0f)
            fH = 0.0f;
        
        fH *= 6.0; // sector 0 to 5
        fI = floor( fH ); // integer part of h (0,1,2,3,4,5 or 6)
        iI = (int) fH; // " " " "
        fF = fH - fI; // factorial part of h (0 to 1)
        
        p = fV * ( 1.0f - fS );
        q = fV * ( 1.0f - fS * fF );
        t = fV * ( 1.0f - fS * ( 1.0f - fF ) );
        
        switch( iI ) {
            case 0:
                fR = fV;
                fG = t;
                fB = p;
                break;
            case 1:
                fR = q;
                fG = fV;
                fB = p;
                break;
            case 2:
                fR = p;
                fG = fV;
                fB = t;
                break;
            case 3:
                fR = p;
                fG = q;
                fB = fV;
                break;
            case 4:
                fR = t;
                fG = p;
                fB = fV;
                break;
            default: // case 5 (or 6):
                fR = fV;
                fG = p;
                fB = q;
                break;
        }
    }
    
    // Convert from floats to 8-bit integers
    int bR = (int)(fR * FLOAT_TO_BYTE);
    int bG = (int)(fG * FLOAT_TO_BYTE);
    int bB = (int)(fB * FLOAT_TO_BYTE);
    
    // Clip the values to make sure it fits within the 8bits.
    if (bR > 255)
        bR = 255;
    if (bR < 0)
        bR = 0;
    if (bG > 255)
        bG = 255;
    if (bG < 0)
        bG = 0;
    if (bB > 255)
        bB = 255;
    if (bB < 0)
        bB = 0;
    
    return Scalar(bB,bG,bR);
}

float Greenness::compareLCCValues(int H){
    
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
    float lccval = compareLCCValues(aveH/pixelct);
    lccval = floor(lccval * 1000.0) / 1000.0;
    
    
    cvtColor(image, image, CV_HSV2BGR);
    
    char name[30];
//    sprintf(name,"Average H=%d",aveH/pixelct);
//    putText(image,name, Point(40,80) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
//    
//    sprintf(name,"Average S=%d",aveS/pixelct);
//    putText(image,name, Point(40,160) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
//    
//    sprintf(name,"Average V=%d",aveV/pixelct);
//    putText(image,name, Point(40,240) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
//    
//    sprintf(name,"Pixels=%d",pixelct);
//    putText(image,name, Point(40,320) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
//    
//    sprintf(name,"LCC Value: %.1f", lccval);
//    putText(image,name, Point(40,400) , FONT_HERSHEY_SIMPLEX, 2, Scalar(0,0,0), 2,8,false );
//    
//    rectangle(image, Point(40,420), Point(140,520), HSVtoRGBcvScalar(aveH/pixelct, aveS/pixelct, aveV/pixelct),CV_FILLED);
    
    result = image;
    
    return lccval;
    
}