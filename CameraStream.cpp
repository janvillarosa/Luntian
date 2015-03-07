#include "CameraStream.h"
#include "Image.h"
#include "PhenotypicData.h"
#include <thread>

//Camera Stream Methods
CameraStream::CameraStream(int camera_id, string camera_username, string camera_password, string ipAddress){
	this->camera_id = camera_id;
	this->camera_username = camera_username;
	this->camera_password = camera_password;
	this->ipAddress = ipAddress;
}

//Image Processing Thread
void processImage(Image* img, sql::Connection *con){
	sql::Statement *stmt = con->createStatement();
	sql::ResultSet *res;



	//Image Processing
	img->setHeightPath(img->getRawPath());
	img->setTillerPath(img->getRawPath());
	img->setGreenPath(img->getRawPath());
	img->setBiomassPath(img->getRawPath());

	img->insertToDatabase(con);

	std::stringstream sstm;
	sstm << "SELECT ID FROM image WHERE raw_path ='" << img->getRawPath() << "'";
	res = stmt->executeQuery(sstm.str());

	res->next();
	PhenotypicData *data = new PhenotypicData(res->getInt("ID"));

	data->setHeight(1.123);
	data->setTillerCount(19);
	data->setGreeness(9.642);
	data->setDiameter(0.532);
	data->setBiomass(4.183);

	data->insertToDatabase(con);

	cout << "Image Saved!" << endl;
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

					//Get plant details
					sql::Statement *stmt = con->createStatement();
					sql::ResultSet *res;
					

					int left_plant_id;
					int right_plant_id;
					string left_plant_stage;
					string right_plant_stage;
					string left_plant_name;
					string right_plant_name;

					std::stringstream sstm;
					sstm << "SELECT plant.ID, plant.Plant_Name, plant.Plant_Stage FROM plant INNER JOIN camera ON camera.Current_Left_Plant_ID = plant.ID WHERE camera.ID =" << camera_id;
					res = stmt->executeQuery(sstm.str());

					if (res->next()){
						left_plant_id = res->getInt("ID");
						left_plant_name = res->getString("Plant_Name");
						left_plant_stage = res->getString("Plant_Stage");

						std::stringstream image_name;
						image_name << left_plant_name << "-" << next_appointment->getDateTimeFileString() << ".jpeg";
						imwrite(image_name.str(), frame);//replace frame with splted image
						Image* leftImage = new Image(left_plant_id, next_appointment->getDateTaken(), left_plant_stage, image_name.str());//replace image_name from splited image
						processImage(leftImage, con);
					}

					delete res;

					std::stringstream sstm2;
					sstm2 << "SELECT plant.ID, plant.Plant_Name, plant.Plant_Stage FROM plant INNER JOIN camera ON camera.Current_Right_Plant_ID = plant.ID WHERE camera.ID =" << camera_id;
					res = stmt->executeQuery(sstm2.str());

					if (res->next()){
						right_plant_id = res->getInt("ID");
						right_plant_name = res->getString("Plant_Name");
						right_plant_stage = res->getString("Plant_Stage");

						std::stringstream image_name2;
						image_name2 << right_plant_name << "-" << next_appointment->getDateTimeFileString() << ".jpeg";
						imwrite(image_name2.str(), frame);//replace frame with splted image
						Image* rightImage = new Image(right_plant_id, next_appointment->getDateTaken(), right_plant_stage, image_name2.str());//replace image_name from splited image
						processImage(rightImage, con);
					}

					delete res;

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

void CameraStream::checkNextAppointment(sql::Connection *con){
	/*Check if new next appointment is needed*/
	sql::Statement *stmt = con->createStatement();
	sql::ResultSet *res;

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