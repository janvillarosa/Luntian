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
#include <iostream>

using namespace cv;
using namespace std;

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
    
    Rect roi(600, 2300, 700, 300); //temporary placement
    Mat src_roi = biomass_segment(roi);        //crops lower portion of the plant for further processing
    src_roi.copyTo(biomass_segment);      // processed image will be cropped at the sides using cropImage()
    
    /*
    Mat src_gray;                               //temporary placement
    cvtColor(filtered, src_gray, CV_RGB2GRAY);  //of this code block
    threshold(src_gray, filtered, 170, 255, 0); //here. Code runs.
     */
    
    Biomass b_instance;
    int aveWidth = b_instance.getPlantWidth(biomass_segment);
    cout << aveWidth << endl;

    waitKey();
}
