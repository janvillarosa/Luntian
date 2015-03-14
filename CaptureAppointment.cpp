#include "CaptureAppointment.h"

//Camera Appointment Class

	Camera_Appointment::Camera_Appointment(){
		this->id = NULL;
		this->date_taken = "";
		this->camera_id = NULL;
		this->interval = "";
	}

	Camera_Appointment::Camera_Appointment(int id, sql::SQLString date_taken, int camera_id, sql::SQLString interval){
		this->id = id;
		this->date_taken = date_taken;
		this->camera_id = camera_id;
		this->interval = interval;
	}

	struct tm parseDateString(const char* date){
		struct tm time_tm;

		int year, month;
		sscanf_s(date, "%d-%d-%d %d:%d:%d", &year, &month, &(time_tm.tm_mday), &(time_tm.tm_hour), &(time_tm.tm_min), &(time_tm.tm_sec));
		time_tm.tm_year = year - 1900;
		time_tm.tm_mon = month - 1;

		return time_tm;
	}

	int Camera_Appointment::checkIfPastCurrentTime(){
		time_t current_time = time(NULL);
		struct tm time_tm = parseDateString(date_taken.c_str());
		time_t next_appointment_time = mktime(&time_tm);

		if (difftime(next_appointment_time, current_time) <= 0){
			return 1;
		}
		else{
			return 0;
		}
	}

	//Compare datetime strings
	int Camera_Appointment::compareDate(string date){
		string str = date_taken.asStdString();
		return date.compare(str);
	}

	//Gets the next datetime base of interval
	string Camera_Appointment::getNextIntervalDateTime(){
		if (interval.compare("Once")){
			/*Set new appointment if interval is not once*/
			struct tm time_tm = parseDateString(date_taken.c_str());

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

	/*Create New Appointment if Interval is not Once*/
	void Camera_Appointment::createNewAppointmentBasedFromInterval(sql::Connection *con){
		sql::PreparedStatement  *prep_stmt;

		string new_datetime = getNextIntervalDateTime();

		prep_stmt = con->prepareStatement("INSERT INTO `camera_appointment` (`Date_Taken`, `Camera_ID`, `Interval`) VALUES (?, ?, ?)");
		prep_stmt->setString(1, new_datetime);
		prep_stmt->setInt(2, getCameraId());
		prep_stmt->setString(3, getInterval());
		prep_stmt->execute();

		delete prep_stmt;
	}

	void Camera_Appointment::printDetails(){
		cout << "Next appointment: " << getDateTaken() << endl;
		cout << "ID: " << getId() << endl;
		cout << "Camera_ID: " << getCameraId() << endl;
		cout << "Interval: " << getInterval() << endl;
	}

	//Getters
	int Camera_Appointment::getId(){ return id; }
	string Camera_Appointment::getDateTaken(){ return date_taken.asStdString(); }
	int Camera_Appointment::getCameraId(){ return camera_id; }
	string Camera_Appointment::getInterval(){ return interval.asStdString(); }

	string Camera_Appointment::getDateTimeFileString(){
		struct tm time_tm = parseDateString(date_taken.c_str());

		/*Convert to String*/
		std::string date_str;
		char date_char[20] = "";
		strftime(date_char, 20, "%Y-%m-%d %H-%M-%S", &time_tm);
		date_str = "";
		date_str.append(date_char);

		return date_str;
	}
