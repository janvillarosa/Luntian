// Standard includes
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <time.h>

using namespace std;

// Connector Includes

#include "cppconnection/driver.h"
#include "cppconnection/exception.h"
#include "cppconnection/resultset.h"
#include "cppconnection/statement.h"
#include "cppconnection/prepared_statement.h"


// Link to the Connector/C++ library
#pragma comment(lib, "mysqlcppconn.lib")

// Connection details
string server = "localhost";
string username = "root";
string password = "root";

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
				int year, month, hour, min, sec;
				sscanf_s(date_taken.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &(time_tm.tm_mday), &(time_tm.tm_hour), &(time_tm.tm_min), &(time_tm.tm_sec));
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

		//Getters
		int getId(){ return id; }
		string getDateTaken(){ return date_taken.asStdString(); }
		int getCameraId(){ return camera_id; }
		string getInterval(){ return interval.asStdString(); }
};


int main(void)
{

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(server, username, password);
		/* Connect to the MySQL test database */
		con->setSchema("Butil");
		stmt = con->createStatement();

		string select_next_appointment_query = "SELECT * FROM `camera_appointment` WHERE `Date_taken` > now() order by Date_Taken asc LIMIT 1;";//QUERY

		/*Check until current time*/
		std::string current_time_str;
		char current_time[20] = "";
		time_t current_raw_time;
		struct tm time_struct;

		/*Camera Appointment Class Declaration*/
		Camera_Appointment *next_appointment = NULL;


		do{
			/*Retrieve Current Time*/
			current_raw_time = time(NULL);
			localtime_s(&time_struct, &current_raw_time);

			strftime(current_time, 20, "%Y-%m-%d %H:%M:%S", &time_struct);
			current_time_str = "";
			current_time_str.append(current_time);
			
			/*Compare Current time and Next Appointment time*/
			if (next_appointment != NULL){
				if (!next_appointment->compareDate(current_time_str)){
					//EXECUTE CAPTURE HERE
					cout << "Execute Capture! " << endl;

					if (!(next_appointment->getInterval().compare("Daily") && next_appointment->getInterval().compare("Weekly"))){
						sql::PreparedStatement  *prep_stmt;

						string new_datetime = next_appointment->getNextIntervalDateTime();
						//string insert_new_appointment_query = "INSERT INTO `camera_appointment` (`Date_Taken`, `Camera_ID`, `Interval`) VALUES ('" + new_datetime + "'," + next_appointment->getCameraId + ",'" + next_appointment->getInterval + "');";
						
						prep_stmt = con->prepareStatement("INSERT INTO `camera_appointment` (`Date_Taken`, `Camera_ID`, `Interval`) VALUES (?, ?, ?)");
						prep_stmt->setString(1, new_datetime);
						prep_stmt->setInt(2, next_appointment->getCameraId());
						prep_stmt->setString(3, next_appointment->getInterval());
						prep_stmt->execute();

						delete prep_stmt;
					}

					delete next_appointment;
					next_appointment = NULL;
				}
			}

			/*Check if new next appointment is needed*/
			if (next_appointment == NULL){

				res = stmt->executeQuery(select_next_appointment_query);
				if (res->next()){
					next_appointment = new Camera_Appointment(res->getInt("ID"), res->getString("Date_Taken"), res->getInt("Camera_ID"), res->getString("Interval"));

					cout << "Next appointment: " << next_appointment->getDateTaken() << endl;
					cout << "ID: " << next_appointment->getId() << endl;
					cout << "Camera_ID: " << next_appointment->getCameraId() << endl;
					cout << "Interval: " << next_appointment->getInterval() << endl;
				}
				delete res;
			}
		}while (1);

		_getch();
		delete res;
		delete stmt;
		delete con;

	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getch();
	}

	cout << endl;

	return 0;
}