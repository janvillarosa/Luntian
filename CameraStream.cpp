
#include "Preprocessor.h"
#include "Greenness.h"
#include "Biomass.h"
#include "CameraStream.h"
#include "Image.h"
#include "PhenotypicData.h"
#include <thread>

//pointing pointing to web image directory
const String WEB_IMAGE_DIRECTORY = "assets/images/";

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

	PhenotypicData *data = new PhenotypicData();

	//height and tiller set to 1 for now
	data->setHeight(1);
	data->setTillerCount(1);

	/*******************IMAGE PROCESSING MODULES***************************/

	Preprocessor pp_instance;
	String tempDir;
	pp_instance.setSrc(img->getImage());//Image passed here for Preproccessing greenness and biomass
	Mat wBal = pp_instance.whiteBal();

	Mat  biomass_segment = pp_instance.bin_segment(wBal);
	Mat  green_segment = pp_instance.cropRGBImage(pp_instance.noisefilter(pp_instance.rgb_segment(pp_instance.segment(wBal), wBal)));

	imwrite(img->getBiomassPath(), biomass_segment);

	Greenness g_instance;

	float greenval = g_instance.greenness(green_segment); //greenness float output

	data->setGreeness(greenval);//Set greenness value for database

	green_segment = g_instance.getResult();

	imwrite(img->getGreennessPath(), green_segment);

	Biomass b_instance;
	int potPixelCount = 210;
	double potActualInchDimension = 10;

	double aveWidth = b_instance.getPlantWidth(biomass_segment);
	double diameter = b_instance.convertPixelToCm(b_instance.computePlantRadius(aveWidth), potPixelCount, potActualInchDimension);
	double biomassval = b_instance.computePlantBiomass(diameter, data->getHeight());//Height is set to 1 by default if Seight Module is not integrated
	
	data->setBiomass(biomassval);
	data->setDiameter(diameter);

	/*********************************************************************/

	img->insertToDatabase(con);

	std::stringstream sstm;
	sstm << "SELECT ID FROM image WHERE raw_path ='" << img->getRawPath() << "'";
	res = stmt->executeQuery(sstm.str());

	res->next();
	data->setImageID(res->getInt("ID"));

	data->insertToDatabase(con);

	cout << "Image " << img->getRawPath() << " Saved!" << endl;
}

void CameraStream::checkAppointment(sql::Connection *con){
		/*Compare Current time and Next Appointment time*/
		if (next_appointment != NULL){
			if (next_appointment->checkIfPastCurrentTime()){
				/*EXECUTE CAPTURE*/
				VideoCapture cap("http://" + camera_username + ":" + camera_password + "@" + ipAddress + "/image/jpeg.cgi?user=" + camera_username + "&password=" + camera_password + "&channel=0&.jpeg");//Video capture url

				if (cap.isOpened()){
					cap >> frame;

					//Split image
					Mat leftFrame;
					Mat rightFrame;

					Mat orig = frame;
					Rect roi(0, 0, orig.cols / 2, orig.rows);
					Rect roi_2(orig.cols / 2, 0, orig.cols / 2, orig.rows);

					Mat image_roi = orig(roi);
					image_roi.copyTo(leftFrame);
					image_roi = orig(roi_2);
					image_roi.copyTo(rightFrame);

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

					//Process Left Plant
					if (res->next()){
						left_plant_id = res->getInt("ID");
						left_plant_name = res->getString("Plant_Name");
						left_plant_stage = res->getString("Plant_Stage");

						std::stringstream image_name;
						image_name << WEB_IMAGE_DIRECTORY << left_plant_name << "-" << next_appointment->getDateTimeFileString();
						String raw_path = image_name.str();
						raw_path.append(".jpg");
						imwrite(raw_path, leftFrame);
						Image* leftImage = new Image(leftFrame, left_plant_id, next_appointment->getDateTaken(), left_plant_stage, image_name.str());
						processImage(leftImage, con);
					}

					delete res;

					std::stringstream sstm2;
					sstm2 << "SELECT plant.ID, plant.Plant_Name, plant.Plant_Stage FROM plant INNER JOIN camera ON camera.Current_Right_Plant_ID = plant.ID WHERE camera.ID =" << camera_id;
					res = stmt->executeQuery(sstm2.str());

					//Process Right Plant
					if (res->next()){
						right_plant_id = res->getInt("ID");
						right_plant_name = res->getString("Plant_Name");
						right_plant_stage = res->getString("Plant_Stage");

						std::stringstream image_name2;
						image_name2 << WEB_IMAGE_DIRECTORY << right_plant_name << "-" << next_appointment->getDateTimeFileString();
						String raw_path = image_name2.str();
						raw_path.append(".jpg");
						imwrite(raw_path, rightFrame);
						Image* rightImage = new Image(rightFrame, right_plant_id, next_appointment->getDateTaken(), right_plant_stage, image_name2.str());
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