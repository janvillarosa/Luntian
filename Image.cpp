#include "Image.h"

Image::Image(Mat img, int plant_id, string date_taken, string plant_stage, string image_name){
	String height_path = image_name;
	String tiller_path = image_name;
	String greenness_path = image_name;
	String biomass_path = image_name;
	image_name.append(".jpg");
	height_path.append("-height.jpg");
	tiller_path.append("-tiller.jpg");
	greenness_path.append("-greenness.jpg");
	biomass_path.append("-biomass.jpg");

	this->img = img;
	this->plant_id = plant_id;
	this->date_taken = date_taken;
	this->plant_stage = plant_stage;
	this->raw_path = raw_path;
	this->processed_height_path = height_path;
	this->processed_tiller_path = tiller_path;
	this->processed_greenness_path = greenness_path;
	this->processed_biomass_path = biomass_path;
}

void Image::insertToDatabase(sql::Connection *con){
	sql::PreparedStatement  *prep_stmt;

	prep_stmt = con->prepareStatement("INSERT INTO `image` (`Plant_ID`, `Date_Taken`, `Plant_Stage`, `Comments`, `Raw_Path`, `Processed_Height_Path`, `Processed_Tiller_Path`, `Processed_Greenness_Path`, `Processed_Biomass_Path`) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");//EDIT QUERY
	prep_stmt->setInt(1, this->plant_id);
	prep_stmt->setString(2, this->date_taken);
	prep_stmt->setString(3, this->plant_stage);
	prep_stmt->setString(4, "");
	prep_stmt->setString(5, this->raw_path);
	prep_stmt->setString(6, this->processed_height_path);
	prep_stmt->setString(7, this->processed_tiller_path);
	prep_stmt->setString(8, this->processed_greenness_path);
	prep_stmt->setString(9, this->processed_biomass_path);
	prep_stmt->execute();

	delete prep_stmt;
}

//GETTERS
String Image::getHeightPath(){
	return this->processed_height_path;
}
String Image::getTillerPath(){
	return this->processed_tiller_path;
}
String Image::getGreennessPath(){
	return this->processed_greenness_path;
}
String Image::getBiomassPath(){
	return this->processed_biomass_path;
}
String Image::getRawPath(){
	return this->raw_path;
}
Mat Image::getImage(){
	return this->img;
}
