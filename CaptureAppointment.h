// Standard includes
#ifndef __CAPTUREAPPOINTMENT_H_INCLUDED__ 
#define __CAPTUREAPPOINTMENT_H_INCLUDED__

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

//Camera Appointment Class
class Camera_Appointment {
private://Atrributes
	int id;
	sql::SQLString date_taken;
	int camera_id;
	sql::SQLString interval;

public://Methods
	Camera_Appointment();
	Camera_Appointment(int id, sql::SQLString date_taken, int camera_id, sql::SQLString interval);
	int checkIfPastCurrentTime();
	int compareDate(string date);
	string getNextIntervalDateTime();
	void createNewAppointmentBasedFromInterval(sql::Connection *con);
	void printDetails();

	int getId();
	string getDateTaken();
	int getCameraId();
	string getInterval();

	string getDateTimeFileString();
};

#endif