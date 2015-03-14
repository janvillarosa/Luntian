#include "PhenotypicData.h"

void PhenotypicData::insertToDatabase(sql::Connection *con){
	sql::PreparedStatement  *prep_stmt;

	prep_stmt = con->prepareStatement("INSERT INTO `phenotypic_data` (`Image_ID`, `Height`, `Tiller_Count`,`Greenness`, `Diameter`, `Biomass`) VALUES (?, ?, ?, ?, ?, ?)");//EDIT QUERY
	prep_stmt->setInt(1, this->image_id);
	prep_stmt->setDouble(2, this->Height);
	prep_stmt->setInt(3, this->Tiller_Count);
	prep_stmt->setDouble(4, this->Greeness);
	prep_stmt->setDouble(5, this->Diameter);
	prep_stmt->setDouble(6, this->Biomass);
	prep_stmt->execute();

	delete prep_stmt;
}

//SETTERS AND GETTERS
void PhenotypicData::setHeight(double Height){
	this->Height = Height;
}
void PhenotypicData::setTillerCount(int Tiller_Count){
	this->Tiller_Count = Tiller_Count;
}
void PhenotypicData::setGreeness(double Greeness){
	this->Greeness = Greeness;
}
void PhenotypicData::setDiameter(double Diameter){
	this->Diameter = Diameter;
}
void PhenotypicData::setBiomass(double Biomass){
	this->Biomass = Biomass;
}
void PhenotypicData::setImageID(int image_id){
	this->image_id = image_id;
}

double PhenotypicData::getHeight(){
	return this->Height;
}
int PhenotypicData::getTillerCount(){
	return this->Tiller_Count;
}
double PhenotypicData::getGreeness(){
	return this->Greeness;
}
double PhenotypicData::getDiameter(){
	return this->Diameter;
}
double PhenotypicData::getBiomass(){
	return this->Biomass;
}

