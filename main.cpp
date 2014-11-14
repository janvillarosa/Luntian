//
//  main.cpp
//  OpenCVDEmo
//
//  Created by Jan Villarosa on 11/3/14.
//  Copyright (c) 2014 Jan Villarosa. All rights reserved.
//

#include "Preprocessor.cpp"
#include "Greenness.cpp"
#include "Biomass.cpp"

using namespace cv;

int main(int argc, char *argv[]){
    Mat src;
    src = imread("/Users/janvillarosa/Desktop/IR64-055.JPG",1);
    Preprocessor pp_instance;
    pp_instance.setSrc(src);
    Mat filtered = pp_instance.noisefilter(pp_instance.segment(pp_instance.whiteBal()));
    
    Greenness g_instance;
    g_instance.greenness(filtered);

    Biomass b_instance;
    b_instance.getPlantWidth(pp_instance.cropImage(filtered));
    waitKey();
}
