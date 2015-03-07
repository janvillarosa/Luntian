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
	double Greeness;
	double Diameter;
	double Biomass;

public:
	PhenotypicData(int image_id);
	void insertToDatabase(sql::Connection *con);

	void setHeight(double Height);
	void setTillerCount(int Tiller_Count);
	void setGreeness(double Greeness);
	void setDiameter(double Diameter);
	void setBiomass(double Biomass);
};

#endif