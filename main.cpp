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
    double h1[] = { 75.5,67,72.5,76.5,70.5,70,80,83,76,77.5,70,76.5,75.5,63.5,59,65,71.5,73,68.5,70.5,87,90.5,87.5,96.5,0,0,0,0,0,0,0,32,0,0,31,87,94,
        87,105,80,95,100,100,97,96,90,92,88,105,90,100,96,92,90,91,87,92,90,93,90}; //while db is not yet connected
    
    ofstream myfile, gfile;
    myfile.open("/Users/janvillarosa/Documents/Luntian/radius.txt"); //FOR TESTING
    gfile.open("/Users/janvillarosa/Documents/Luntian/greenness.txt");

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
            
            string dir = "/Users/janvillarosa/Documents/Luntian/Pre-processed/IR64-0"; //white-bal image path (for testing purposes)
            if (i < 10){
                dir.append("0");
            }
            dir.append(to_string(i));
            dir.append("-wbal.JPG");
            
            imwrite(dir, wBal);
        
            Mat  biomass_segment = pp_instance.bin_segment(wBal);
            Mat  green_segment = pp_instance.noisefilter(pp_instance.rgb_segment(pp_instance.segment(wBal),wBal));
            
            dir = "/Users/janvillarosa/Documents/Luntian/Pre-processed/IR64-0"; //image segmentation
            if (i < 10){
                dir.append("0");
            }
            dir.append(to_string(i));
            dir.append("-rgb.JPG");
            
            imwrite(dir, green_segment);
        
        
            Greenness g_instance;
            
            float greenval = g_instance.greenness(green_segment); //greenness float output
            green_segment = g_instance.getResult();
            
            gfile << "IR64-0" << i << " -- LCC value: " << greenval << " \n";
            
            dir = "/Users/janvillarosa/Documents/Luntian/Greenness/IR64-0"; //greenness image output
            if (i < 10){
                dir.append("0");
            }
            dir.append(to_string(i));
            dir.append(".JPG");
            
            imwrite(dir, green_segment);

            
            
            biomass_segment=pp_instance.cropImage(biomass_segment);
            
            dir = "/Users/janvillarosa/Documents/Luntian/Pre-processed/IR64-0";
            if (i < 10){
                dir.append("0");
            }
            dir.append(to_string(i));
            dir.append("-bin.JPG");
            
            imwrite(dir, biomass_segment);
            
            Biomass b_instance;

            double aveWidth = b_instance.getPlantWidth(biomass_segment);
            cout << b_instance.computePlantBiomass(b_instance.convertPixelToCm(b_instance.computePlantRadius(aveWidth)), h1[i - 1]) << endl;
            myfile << aveWidth << " " << i << ": " << b_instance.computePlantRadius(aveWidth) << " pixels -> " << b_instance.convertPixelToCm(b_instance.computePlantRadius(aveWidth)) << " cm biomass (longest tiller): " << b_instance.computePlantBiomass(b_instance.convertPixelToCm(b_instance.computePlantRadius(aveWidth)), h1[i - 1]) << " cm \n";
        }
    }
    myfile.close();
    gfile.close();
}
