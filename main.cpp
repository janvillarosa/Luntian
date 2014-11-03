//
//  main.cpp
//  OpenCVDEmo
//
//  Created by Jan Villarosa on 11/3/14.
//  Copyright (c) 2014 Jan Villarosa. All rights reserved.
//

#include "Preprocessor.cpp"
#include "Greenness.cpp"

using namespace cv;

int main(int argc, char *argv[]){
    Mat src;
    src = imread("/Users/janvillarosa/Pictures/sample.jpg",1);
    imshow("Original",src);
    Preprocessor pp_instance;
    pp_instance.setSrc(src);
    Mat filtered = pp_instance.noisefilter(pp_instance.segment(pp_instance.whiteBal()));
    
    Greenness g_instance;
    g_instance.greenness(filtered);
    waitKey();
}