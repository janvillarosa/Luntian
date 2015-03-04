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
//#include "Capturemodule.cpp"
#include "DatabaseAccess.cpp"
#include <iostream>

using namespace cv;
using namespace std;

const String FILE_DIR_SOURCE = "/Users/janvillarosa/Desktop/test images/";
const String FILE_DIR_PREPROCESSING = "/Users/janvillarosa/git/web-butil/public/Phenotypic Images/Preprocessing/IR64-0";
const String FILE_DIR_GREENNESS = "/Users/janvillarosa/git/web-butil/public/Phenotypic Images/Greenness/IR64-0";

int main(int argc, char *argv[]){
    Mat src;
    String tempDir;
    int diagnosticMode = 1;
    int plantID = 0;
    
    double h1[] = { 75.5,67,72.5,76.5,70.5,70,80,83,76,77.5,70,76.5,75.5,63.5,59,65,71.5,73,68.5,70.5,87,90.5,87.5,96.5,0,0,0,0,0,0,0,32,0,0,31,87,94,
        87,105,80,95,100,100,97,96,90,92,88,105,90,100,96,92,90,91,87,92,90,93,90}; //while db is not yet connected
    
    if(diagnosticMode != 1){
        
        //CAPTURE MODE: Uses the camera setup for running the phenotyping algorithms
        
        //CaptureModule cp_instance;
        //cp_instance.testCapture();
        //src = cp_instance.getFrame();
        
        int plantHeight = 0; //hardcoded to 0 because height function is not integrated yet
        
        DatabaseAccess db_instance;
        
        Preprocessor pp_instance;
        pp_instance.setSrc(src);
        Mat wBal = pp_instance.whiteBal();
        
        tempDir = FILE_DIR_PREPROCESSING; //white-balance image output (for testing purposes)

        tempDir.append(to_string(plantID));
        tempDir.append("-wbal.JPG");
        
        imwrite(tempDir, wBal);
        
        db_instance.setPrepro_segment(tempDir);
        
        Mat  biomass_segment = pp_instance.bin_segment(wBal);
        
        tempDir = FILE_DIR_PREPROCESSING; //white-balance image output (for testing purposes)
        
        tempDir.append(to_string(plantID));
        tempDir.append("-bin.JPG");
        
        imwrite(tempDir, biomass_segment);
        
        db_instance.setBiomass_segment(tempDir);
        
        Mat  green_segment = pp_instance.noisefilter(pp_instance.rgb_segment(pp_instance.segment(wBal),wBal));
        
        Greenness g_instance;
        
        float greenval = g_instance.greenness(green_segment); //greenness float output
        green_segment = g_instance.getResult();
        
        tempDir = FILE_DIR_GREENNESS; //white-balance image output (for testing purposes)
        
        tempDir.append(to_string(plantID));
        tempDir.append("-rgb.JPG");
        
        imwrite(tempDir, green_segment);
        
        db_instance.setGreenness_segment(tempDir);
        
        //IMPORTANT: BEFORE RETRIEVING BIOMASS, BE SURE TO INTEGRATE THE HEIGHT ALGORITHM FROM SEIGHT
        
        Biomass b_instance;
        
        double aveWidth = b_instance.getPlantWidth(biomass_segment);
        double biomassval = b_instance.computePlantBiomass(b_instance.convertPixelToCm(b_instance.computePlantRadius(aveWidth)), h1[plantHeight - 1]);
        cout << biomassval << endl;
        
        //HARDCODED HEIGHT AND TILLER VALUES, THIS WILL BE GONE WHEN INTEGRATION WITH SEIGHT IS FINISHED
        double heightval = 0;
        double tillerval = 0;
        
        db_instance.setValues(heightval, tillerval, greenval, aveWidth, biomassval);
        db_instance.insertToDB();
        
    } else {
        
        //DIAGNOSTIC MODE: retrieves images from a directory.
        //FOR DIAGNOSTICS ONLY. USE CAMERA SETUP IF POSSIBLE
        
        ofstream myfile, gfile;
        myfile.open("/Users/janvillarosa/git/web-butil/public/Phenotypic Images/radius.txt"); //FOR TESTING
        gfile.open("/Users/janvillarosa/git/web-butil/public/Phenotypic Images/greenness.txt");
        
        for (int i = 1; i < 61; i++){
            tempDir = FILE_DIR_SOURCE;
            
            if (i < 10){
                tempDir.append("0");
            }
            tempDir.append(to_string(i));
            tempDir.append(".JPG");
            
            src = imread(tempDir, 1);
            if (src.empty() == false){
                
                DatabaseAccess db_instance;
                
                Preprocessor pp_instance;
                pp_instance.setSrc(src);
                Mat wBal = pp_instance.whiteBal();
                
                tempDir = FILE_DIR_PREPROCESSING; //white-balance image output (for testing purposes)
                if (i < 10){
                    tempDir.append("0");
                }
                tempDir.append(to_string(i));
                tempDir.append("-wbal.JPG");
                
                imwrite(tempDir, wBal);
                
                db_instance.setPrepro_segment(tempDir);
                
                Mat  biomass_segment = pp_instance.bin_segment(wBal);
                Mat  green_segment = pp_instance.noisefilter(pp_instance.rgb_segment(pp_instance.segment(wBal),wBal));
                
                tempDir = FILE_DIR_PREPROCESSING; //image segmentation image output
                if (i < 10){
                    tempDir.append("0");
                }
                tempDir.append(to_string(i));
                tempDir.append("-rgb.JPG");
                
                imwrite(tempDir, green_segment);
                
                db_instance.setGreenness_segment(tempDir);
                
                biomass_segment=pp_instance.cropImage(biomass_segment);
                
                tempDir = FILE_DIR_PREPROCESSING; // biomass preprocessing image output
                if (i < 10){
                    tempDir.append("0");
                }
                tempDir.append(to_string(i));
                tempDir.append("-bin.JPG");
                
                imwrite(tempDir, biomass_segment);
                
                db_instance.setBiomass_segment(tempDir);
                
                Greenness g_instance;
                
                float greenval = g_instance.greenness(green_segment); //greenness float output
                green_segment = g_instance.getResult();
                
                gfile << "IR64-0" << i << " -- LCC value: " << greenval << " \n";
                
                tempDir = FILE_DIR_GREENNESS; //greenness image output
                if (i < 10){
                    tempDir.append("0");
                }
                tempDir.append(to_string(i));
                tempDir.append(".JPG");
                
                imwrite(tempDir, green_segment);
                
                
                Biomass b_instance;
                
                double aveWidth = b_instance.getPlantWidth(biomass_segment);
                double biomassval = b_instance.computePlantBiomass(b_instance.convertPixelToCm(b_instance.computePlantRadius(aveWidth)), h1[i - 1]);
                cout << biomassval << endl;
                myfile << aveWidth << " " << i << ": " << b_instance.computePlantRadius(aveWidth) << " pixels -> " << b_instance.convertPixelToCm(b_instance.computePlantRadius(aveWidth)) << " cm biomass (longest tiller): " << biomassval << " cm \n";
                
                //HARDCODED HEIGHT AND TILLER VALUES, THIS WILL BE GONE WHEN INTEGRATION WITH SEIGHT IS FINISHED
                double heightval = 0;
                double tillerval = 0;
                
                db_instance.setValues(heightval, tillerval, greenval, aveWidth, biomassval);
                //db_instance.insertToDB();
            }
        }
        myfile.close();
        gfile.close();
    }
    return 0;
}

void splitImage(Mat image){
    Mat orig = image;
    Rect roi(0, 0, orig.cols/2, orig.rows);
    Rect roi_2(orig.cols/2, 0, orig.cols/2, orig.rows);
    
    Mat image_roi = orig(roi);
    image_roi.copyTo(orig);
    imwrite(FILE_DIR_SOURCE+"sample1", orig);
    
    image_roi = orig(roi_2);
    image_roi.copyTo(orig);
    imwrite(FILE_DIR_SOURCE+"sample2", orig);
}
