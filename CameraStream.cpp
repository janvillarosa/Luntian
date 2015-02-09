#include "CameraStream.h"

//Camera Stream Methods
CameraStream::CameraStream(int camera_id, string camera_username, string camera_password, string ipAddress){
	this->camera_id = camera_id;
	this->camera_username = camera_username;
	this->camera_password = camera_password;
	this->ipAddress = ipAddress;
}

void CameraStream::checkAppointment(sql::Connection *con){
		/*Compare Current time and Next Appointment time*/
		if (next_appointment != NULL){
			if (next_appointment->checkIfPastCurrentTime()){
				/*EXECUTE CAPTURE*/
				VideoCapture cap("http://" + camera_username + ":" + camera_password + "@" + ipAddress + "/image/jpeg.cgi?user=" + camera_username + "&password=" + camera_password + "&channel=0&.jpeg");//Video capture url

				if (cap.isOpened()){
					cap >> frame;

					/*PERFORM IMAGE PROCESSING HERE*/
					std::stringstream image_name;
					image_name << camera_id << "-" << next_appointment->getDateTimeFileString() << ".jpeg";

					imwrite(image_name.str(), frame);
					cout << "Image Saved!" << endl;
				}
				cap.release();
				/*Create new appointment if interval is daily or weekly*/
				if (!(next_appointment->getInterval().compare("Daily") && next_appointment->getInterval().compare("Weekly"))){
					next_appointment->createNewAppointmentBasedFromInterval(con);
				}

				delete next_appointment;
				next_appointment = NULL;
			}
		}
}

void CameraStream::checkNextAppointment(sql::Statement *stmt, sql::ResultSet *res){
	/*Check if new next appointment is needed*/
	std::stringstream sstm;
	sstm << "SELECT * FROM `camera_appointment` WHERE `Date_taken` > now() AND `Camera_ID` = " << camera_id << " order by Date_Taken asc LIMIT 1;";
	string select_next_appointment_query = sstm.str();

	res = stmt->executeQuery(select_next_appointment_query);
	if (res->next()){
		if (next_appointment == NULL){
			cout << endl;
			next_appointment = new Camera_Appointment(res->getInt("ID"), res->getString("Date_Taken"), res->getInt("Camera_ID"), res->getString("Interval"));
			next_appointment->printDetails();
		}
		else if (next_appointment->compareDate(res->getString("Date_Taken").asStdString())){
			cout << endl;
			cout << "New next appointment found!" << endl;
			next_appointment = new Camera_Appointment(res->getInt("ID"), res->getString("Date_Taken"), res->getInt("Camera_ID"), res->getString("Interval"));
			next_appointment->printDetails();
		}
	}
	delete res;
}