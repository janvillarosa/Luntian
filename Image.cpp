#include "Image.h"

Image::Image(int plant_id, string date_taken, string plant_stage, string raw_path){
	this->plant_id = plant_id;
	this->date_taken = date_taken;
	this->plant_stage = plant_stage;
	this->raw_path = raw_path;
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
	prep_stmt->setString(8, this->processed_greeness_path);
	prep_stmt->setString(9, this->processed_biomass_path);
	prep_stmt->execute();

	delete prep_stmt;
}

//SETTERS
void Image::setHeightPath(string path){
	processed_height_path = path;
}
void Image::setTillerPath(string path){
	processed_tiller_path = path;
}
void Image::setGreenPath(string path){
	processed_greeness_path = path;
}
void Image::setBiomassPath(string path){
	processed_biomass_path = path;
}

string Image::getRawPath(){
	return this->raw_path;
}
