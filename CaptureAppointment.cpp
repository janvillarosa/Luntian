// Standard includes
#include <stdlib.h>
//#include <conio.h>
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
	//Constructors
	Camera_Appointment(){
		this->id = NULL;
		this->date_taken = "";
		this->camera_id = NULL;
		this->interval = "";
	}

	Camera_Appointment(int id, sql::SQLString date_taken, int camera_id, sql::SQLString interval){
		this->id = id;
		this->date_taken = date_taken;
		this->camera_id = camera_id;
		this->interval = interval;
	}

	//Compare datetime strings
	int compareDate(string date){
		string str = date_taken.asStdString();
		return date.compare(str);
	}

	//Gets the next datetime base of interval
	string getNextIntervalDateTime(){
		if (interval.compare("Once")){
			/*Set new appointment if interval is not once*/
			struct tm time_tm;

			/*Parse Datetime*/
			int year, month;
			sscanf(date_taken.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &(time_tm.tm_mday), &(time_tm.tm_hour), &(time_tm.tm_min), &(time_tm.tm_sec));
			time_tm.tm_year = year - 1900;
			time_tm.tm_mon = month - 1;

			/*Set new datetime*/
			if (!interval.compare("Daily"))
				time_tm.tm_mday += 1;
			else if (!interval.compare("Weekly"))
				time_tm.tm_mday += 7;

			/*Adjust Overflowing Numbers*/
			mktime(&time_tm);

			/*Convert to String*/
			std::string new_date_str;
			char date_char[20] = "";
			strftime(date_char, 20, "%Y-%m-%d %H:%M:%S", &time_tm);
			new_date_str = "";
			new_date_str.append(date_char);

			return new_date_str;
		}
		else{
			return "";
		}
	}


	void createNewAppointmentBasedFromInterval(sql::Connection *con){
		sql::PreparedStatement  *prep_stmt;

		string new_datetime = getNextIntervalDateTime();

		prep_stmt = con->prepareStatement("INSERT INTO `camera_appointment` (`Date_Taken`, `Camera_ID`, `Interval`) VALUES (?, ?, ?)");
		prep_stmt->setString(1, new_datetime);
		prep_stmt->setInt(2, getCameraId());
		prep_stmt->setString(3, getInterval());
		prep_stmt->execute();

		delete prep_stmt;
	}

	void printDetails(){
		cout << "Next appointment: " << getDateTaken() << endl;
		cout << "ID: " << getId() << endl;
		cout << "Camera_ID: " << getCameraId() << endl;
		cout << "Interval: " << getInterval() << endl;
	}

	//Getters
	int getId(){ return id; }
	string getDateTaken(){ return date_taken.asStdString(); }
	int getCameraId(){ return camera_id; }
	string getInterval(){ return interval.asStdString(); }
};