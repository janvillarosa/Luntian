//
//  DatabaseAccess.cpp
//  Luntian
//
//  Created by Jan Villarosa on 11/27/14.
//  Copyright (c) 2014 Jan Villarosa. All rights reserved.
//

#include <opencv/cv.h>
#include <stdio.h>

// Connector Includes

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace cv;
using namespace std;

class DatabaseAccess {
private:
    float greenval;
    double biomassval,heightval,tillerval,diameterval;
    String prepro_segment, biomass_segment, greenness_segment;
    String Date_Taken, Plant_ID, Plant_Type;
    
    String server = "localhost";
    String username = "root";
    String password = "root";
    
public:
    void setPrepro_segment(String);
    void setBiomass_segment(String);
    void setGreenness_segment(String);
    void setValues(double,double,float,double,double);
    int insertToDB();
};

void DatabaseAccess::setBiomass_segment(String im){
    biomass_segment=im;
}
void DatabaseAccess::setGreenness_segment(String im){
    greenness_segment=im;
}
void DatabaseAccess::setPrepro_segment(String im){
    prepro_segment=im;
}
void DatabaseAccess::setValues(double hval, double tval, float gval, double dval, double bval){
    heightval = hval;
    tillerval = tval;
    greenval = gval;
    diameterval = dval;
    biomassval = bval;
}

int DatabaseAccess::insertToDB(){
    int imgID = 0;
    
    try {
        
        /* Create a connection */
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        
        Plant_ID = "0";
        Plant_Type = "Tillering";
        
        driver = get_driver_instance();
        con = driver->connect(server, username, password);
        /* Connect to the MySQL test database */
        con->setSchema("Butil");
        stmt = con->createStatement();
        
        string insertImg = "INSERT INTO image(Plant_ID,Plant_Type,Processed_Biomass_Path,Processed_Height_Path,Processed_Tiller_Path,Raw_Path) VALUES ('"+ Plant_ID+"','"+ Plant_Type +"','"+ biomass_segment +"','"+ greenness_segment +"','"+ prepro_segment +"');";
        
        stmt->executeQuery(insertImg);
        
        string selectImgID = "SELECT 'ID' FROM 'image' ORDER BY 'ID' DESC";
        
        stmt->executeQuery(selectImgID);
        
        if (res->next()){
            imgID = res->getInt("ID");
        }
        
        
        string insertVal = "INSERT INTO phenotypic_data(Image_ID,Height,Tiller_Count,Greenness,Diameter,Biomass) VALUES ('"+ to_string(imgID) +"','"+ to_string(heightval) +"','"+ to_string(tillerval) +"','"+ to_string(greenval) +"','"+ to_string(diameterval) +"','"+ to_string(biomassval) +"');";
        
        stmt->executeQuery(insertVal);
    } catch (sql::SQLException &e) {
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