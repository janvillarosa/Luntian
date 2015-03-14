#ifndef __IMAGE_H_INCLUDED__ 
#define __IMAGE_H_INCLUDED__ 

// Standard includes
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <time.h>

//Opencv includes
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

// Connector Includes
#include "cppconnection/driver.h"
#include "cppconnection/exception.h"
#include "cppconnection/resultset.h"
#include "cppconnection/statement.h"
#include "cppconnection/prepared_statement.h"

using namespace std;
using namespace cv;

//Image Class
class Image {
private:
	Mat img;
	int plant_id;
	sql::SQLString date_taken;
	string plant_stage;
	string comments;
	string raw_path;
	string processed_height_path;
	string processed_tiller_path;
	string processed_greenness_path;
	string processed_biomass_path;

public:
	Image(Mat img, int plant_id, string date_taken, string plant_stage, string raw_path);
	void insertToDatabase(sql::Connection *con);

	string getRawPath();
	string getHeightPath();
	string getTillerPath();
	string getGreennessPath();
	string getBiomassPath();
	Mat getImage();
};

#endif