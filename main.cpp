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
    
    for (int i = 1; i < 61; i++){
        string x = "/Users/janvillarosa/Dropbox/Butil - IRRI Project/Images/10-30-2014/IR64-0";
   
        if (i < 10){
            x.append("0");
        }
        x.append(to_string(i));
        x.append(".JPG");

        src = imread(x, 1);
        if (src.empty() == false){
            
        
            Preprocessor pp_instance;
            pp_instance.setSrc(src);
            Mat wBal = pp_instance.whiteBal();
        
            Mat  biomass_segment = pp_instance.bin_segment(wBal);
            Mat  green_segment = pp_instance.noisefilter(pp_instance.rgb_segment(pp_instance.segment(wBal),wBal));
            
            string dir = "/Users/janvillarosa/Documents/Luntian/Pre-processed/IR64-0";
            if (i < 10){
                dir.append("0");
            }
            dir.append(to_string(i));
            dir.append("-rgb.JPG");
            
            imwrite(dir, green_segment);
        
        
            Greenness g_instance;
            green_segment = g_instance.greenness(green_segment);
            
            dir = "/Users/janvillarosa/Documents/Luntian/Greenness/IR64-0";
            if (i < 10){
                dir.append("0");
            }
            dir.append(to_string(i));
            dir.append(".JPG");
            
            imwrite(dir, green_segment);

            //Biomass b_instance;
            //b_instance.getPlantWidth(biomass_segment);
            
            
            biomass_segment=pp_instance.cropImage(biomass_segment);
            
            dir = "/Users/janvillarosa/Documents/Luntian/Pre-processed/IR64-0";
            if (i < 10){
                dir.append("0");
            }
            dir.append(to_string(i));
            dir.append("-bin.JPG");
            
            imwrite(dir, biomass_segment);
            
            Biomass b_instance;
            int aveWidth = b_instance.getPlantWidth(biomass_segment);
            cout << aveWidth << endl;
        }
    }
}
