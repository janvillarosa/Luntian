#ifndef __PHENOTYPICDATA_H_INCLUDED__ 
#define __PHENOTYPICDATA_H_INCLUDED__ 

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

//Phenotypic Data Class
class PhenotypicData {
private:
	int image_id;
	double Height;
	int Tiller_Count;
	double Greenness;
	double Diameter;
	double Biomass;

public:
	void insertToDatabase(sql::Connection *con);

	void setHeight(double Height);
	void setTillerCount(int Tiller_Count);
	void setGreenness(double Greenness);
	void setDiameter(double Diameter);
	void setBiomass(double Biomass);
	void setImageID(int image_id);

	double getHeight();
	int getTillerCount();
	double getGreeness();
	double getDiameter();
	double getBiomass();
};

#endif