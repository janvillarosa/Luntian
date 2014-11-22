// Standard includes
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

// Connector Includes

#include "cppconnection/driver.h"
#include "cppconnection/exception.h"
#include "cppconnection/resultset.h"
#include "cppconnection/statement.h"


// Link to the Connector/C++ library
#pragma comment(lib, "mysqlcppconn.lib")

// Connection details
string server = "localhost";
string username = "root";
string password = "root";

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

		/*Retrieve Last Appointment*/
		sql::SQLString sqlString_Date_Taken;
		string next_appointment = "";
		string query = "SELECT * FROM camera_appointment WHERE Date_taken > now() order by Date_Taken asc LIMIT 1;";//QUERY

		stmt = con->createStatement();

		/*Check until current time*/
		std::string current_time_str;
		char current_time[20] = "";
		time_t current_raw_time;
		struct tm time_struct;
		int check_next_appointment;

		do{
			current_raw_time = time(NULL);
			localtime_s(&time_struct, &current_raw_time);

			strftime(current_time, 20, "%Y-%m-%d %H:%M:%S", &time_struct);
			current_time_str = "";
			current_time_str.append(current_time);

			check_next_appointment = next_appointment.compare(current_time_str);

			if (!check_next_appointment){
				//EXECUTE CAPTURE HERE
				cout << "Execute Capture! " << endl;
				next_appointment = "";
			}

			if (next_appointment.empty()){

				res = stmt->executeQuery(query);
				if (res->next()){
					sqlString_Date_Taken = res->getString("Date_Taken");
					next_appointment = sqlString_Date_Taken.asStdString();

					cout << "Next appointment: " << next_appointment << endl;
					cout << "ID: " << res->getInt("ID") << endl;
					cout << "Camera_ID: " << res->getInt("Camera_ID") << endl;
					cout << "Interval: " << res->getInt("Interval") << endl;
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

	return EXIT_SUCCESS;
}