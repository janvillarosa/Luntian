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

// Capture Appointment Object
#include "CameraStream.h"

// Link to the Connector/C++ library
#pragma comment(lib, "mysqlcppconn.lib")

using namespace std;
using namespace cv;

// Connection details
string server = "localhost";
string username = "root";
string password = "root";

int main(void)
{

	try {
		std::vector<CameraStream*> cameras;

		/* Create a connection */
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		driver = get_driver_instance();
		con = driver->connect(server, username, password);
		/* Connect to the MySQL test database */
		con->setSchema("Butil");
		stmt = con->createStatement();

		string select_all_camera = "SELECT * FROM `camera`";//QUERY

		res = stmt->executeQuery(select_all_camera);
		while (res->next()){
			cameras.push_back(new CameraStream(res->getInt("ID"), res->getString("Username"), res->getString("Password"), res->getString("IP_Address")));
			cout << "Camera " << res->getInt("ID") << " properties retrieved" << endl;
		}

		delete res;

		time_t prev_time = time(NULL);
		time_t curr_time;

		do{
			int i;

			curr_time = time(NULL);

			for (i = 0; i < cameras.size(); i++){
				cameras[i]->checkAppointment(con);
			}

			if (curr_time - prev_time >= 10){
				//Check New Cameras
				res = stmt->executeQuery(select_all_camera);
				while (res->next()){
					int found = 0;
					int cameraId = res->getInt("ID");
					for (i = 0; i < cameras.size(); i++){
						if (cameras[i]->getCameraID() == cameraId)
							found = 1;
					}
					if (!found){
						cameras.push_back(new CameraStream(res->getInt("ID"), res->getString("Username"), res->getString("Password"), res->getString("IP_Address")));
						cout << "Camera " << res->getInt("ID") << " properties retrieved" << endl;
					}
				}

				//Check New Appointments
				for (i = 0; i < cameras.size(); i++){
					cameras[i]->checkNextAppointment(con);
				}
				prev_time = time(NULL);
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