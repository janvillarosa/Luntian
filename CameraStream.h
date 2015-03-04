#ifndef __CAMERASTREAM_H_INCLUDED__ 
#define __CAMERASTREAM_H_INCLUDED__

// Standard includes
#include <stdlib.h>
//#include <conio.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <time.h>


//Opencv includes
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Connector Includes
#include <cppconnection/driver.h>
#include <cppconnection/exception.h>
#include <cppconnection/resultset.h>
#include <cppconnection/statement.h>
#include <cppconnection/prepared_statement.h>

#include "CaptureAppointment.h"

using namespace cv;
using namespace std;

//Camera Stream Class
class CameraStream {
private://Atrributes
	int camera_id;

	string ipAddress;
	string camera_username;
	string camera_password;

	Mat frame;
	Camera_Appointment *next_appointment = NULL;

public://Methods
	CameraStream(int camera_id, string camera_username, string camera_password, string ipAddress);
	void checkAppointment(sql::Connection *con);
	void checkNextAppointment(sql::Statement *stmt, sql::ResultSet *res);
};

#endif