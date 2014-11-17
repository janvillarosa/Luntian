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
    src = imread("/Users/janvillarosa/Desktop/IR64-002.JPG",1);
    Preprocessor pp_instance;
    pp_instance.setSrc(src);
    Mat wBal = pp_instance.whiteBal();
    
    Mat  biomass_segment = pp_instance.noisefilter(pp_instance.bin_segment(pp_instance.segment(wBal)));
    Mat  green_segment = pp_instance.noisefilter(pp_instance.rgb_segment(pp_instance.segment(wBal),wBal));
    
    
    
    Greenness g_instance;
    g_instance.greenness(green_segment);

    //Biomass b_instance;
    //b_instance.getPlantWidth(biomass_segment);
    waitKey();
}
