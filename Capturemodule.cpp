// Standard includes
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <ctime>
//#include <conio.h>
#include <time.h>


//Opencv includes
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// Connector Includes
#include "cppconnection/driver.h"
#include "cppconnection/exception.h"
#include "cppconnection/resultset.h"
#include "cppconnection/statement.h"
#include "cppconnection/prepared_statement.h"

// Capture Appointment Object
#include "CaptureAppointment.cpp"

// Link to the Connector/C++ library
#pragma comment(lib, "mysqlcppconn.lib")

using namespace std;
using namespace cv;

class CaptureModule{
    
private:
    Mat frame;
    string server = "localhost";
    string username = "root";
    string password = "root";
    
public:
    int testCapture();
    Mat getFrame();
};

Mat CaptureModule::getFrame(){
    return frame;
}

int CaptureModule::testCapture()
{
    
    try {
        
        //Connect to camera
        string camera_username = "admin";
        string camera_password = "c4rice";
        string ipAddress = "192.168.0.20";
        
        VideoCapture cap("http://" +  ipAddress + "/image/jpeg.cgi?user=" + camera_username + "&password=" + camera_password + "&channel=0&.jpeg");//Video capture url
        
        //VideoCapture cap(0);
        
        if(cap.isOpened()){
            cap >> frame;
            
            /*PERFORM IMAGE PROCESSING HERE*/
            imwrite("/Users/janvillarosa/Documents/Luntian/img.jpeg", frame);
            cout << "Image Saved!" << endl;
        }
        
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
        
        string select_next_appointment_query = "SELECT * FROM `camera_appointment` WHERE `Date_taken` > now() order by Date_Taken asc LIMIT 1;";//QUERY
        
        /*Variables for check time*/
        time_t current_raw_time;
        time_t previous_minute_time = time(NULL);
        struct tm time_struct;
        char current_time[20] = "";
        std::string current_time_str;
        
        
        /*Camera Appointment Class Declaration*/
        Camera_Appointment *next_appointment = NULL;
        
        
        do{
            /*Retrieve Current Time to current_time_str*/
            current_raw_time = time(NULL);
            localtime(&current_raw_time);
            
            strftime(current_time, 20, "%Y-%m-%d %H:%M:%S", &time_struct);
            current_time_str = "";
            current_time_str.append(current_time);
            
            /*Compare Current time and Next Appointment time*/
            if (next_appointment != NULL){
                if (!next_appointment->compareDate(current_time_str)){
                    /*EXECUTE CAPTURE*/
                    if (cap.isOpened()){
                        cap >> frame;
                        
                        /*PERFORM IMAGE PROCESSING HERE*/
                        imwrite("/Users/janvillarosa/Documents/Luntian/image.jpeg", frame);
                        cout << "Image Saved!" << endl;
                    }
                    
                    /*Create new appointment if interval is daily or weekly*/
                    if (!(next_appointment->getInterval().compare("Daily") && next_appointment->getInterval().compare("Weekly"))){
                        next_appointment->createNewAppointmentBasedFromInterval(con);
                    }
                    
                    delete next_appointment;
                    next_appointment = NULL;
                }
            }
            
            /*Check if new next appointment is needed*/
            if (next_appointment == NULL || current_raw_time - previous_minute_time >= 5){
                
                res = stmt->executeQuery(select_next_appointment_query);
                if (res->next()){
                    if (next_appointment == NULL){
                        cout << endl;
                        next_appointment = new Camera_Appointment(res->getInt("ID"), res->getString("Date_Taken"), res->getInt("Camera_ID"), res->getString("Interval"));
                        next_appointment->printDetails();
                    }
                    else if(next_appointment->compareDate(res->getString("Date_Taken").asStdString())){
                        cout << endl;
                        cout << "New next appointment found!" << endl;
                        next_appointment = new Camera_Appointment(res->getInt("ID"), res->getString("Date_Taken"), res->getInt("Camera_ID"), res->getString("Interval"));
                        next_appointment->printDetails();
                    }
                }
                previous_minute_time = time(NULL);
                delete res;
            }
        }while (cap.isOpened());
        
        getchar();
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
        getchar();
    }
    
    cout << endl;
    
    return 0;
}