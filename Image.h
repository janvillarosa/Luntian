#ifndef __IMAGE_H_INCLUDED__ 
#define __IMAGE_H_INCLUDED__ 

// Standard includes
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <time.h>

// Connector Includes
#include "cppconnection/driver.h"
#include "cppconnection/exception.h"
#include "cppconnection/resultset.h"
#include "cppconnection/statement.h"
#include "cppconnection/prepared_statement.h"

using namespace std;

//Image Class
class Image {
private:
	int plant_id;
	sql::SQLString date_taken;
	string plant_stage;
	string comments;
	string raw_path;
	string processed_height_path;
	string processed_tiller_path;
	string processed_greeness_path;
	string processed_biomass_path;

public:
	Image(int plant_id, string date_taken, string plant_stage, string raw_path);
	void insertToDatabase(sql::Connection *con);

	void setHeightPath(string path);
	void setTillerPath(string path);
	void setGreenPath(string path);
	void setBiomassPath(string path);

	string getRawPath();
};

#endif