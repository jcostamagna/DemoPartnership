#include "stdafx.h"
#include "DataManager.h"
#include <iostream>
#include <sstream>
#include <Windows.h>
#include "PostgreSQL.h"

void DataManager::startCollectingData(int id)
{
	this->idSession = id;
	initializeData();
	joy.start();
	while (this->timeToQuit.load()) {
		std::cout << "SESION:" << id << std::endl;
		std::cout << "Estoy corriendo" << std::endl;
		saveData(id, joy.getJoy());
		Sleep(100);
	}
	joy.close();
	std::cout << "TERMINE" << std::endl;
}

void DataManager::initializeData()
{
	PostgreSQL db;
	if (!db.connect()) { return;}
	bool succes = db.doQuery("CREATE TABLE IF NOT EXISTS Data(TIMESTAMP INTEGER, IDSESSION INTEGER REFERENCES Session(Id), DeviceType VARCHAR(40), STEERING INT, ACCELERATOR INT, BRAKE INT"\
		", SLIDER1 INT, PRIMARY KEY (TIMESTAMP, IDSESSION) );");

	if (!succes) { return; }
}

void DataManager::saveData(int id, DIJOYSTATE * js)
{
	PostgreSQL db;
	db.connect();
	std::stringstream queryString;
	time(&this->timer);
	
	
	queryString << "INSERT INTO Data VALUES(" << this->timer << "," << id << ", " << "'Steering'" << 
		"," << js->lX << "," << js->lY << "," << js->lZ << "," << js->rglSlider[0] <<")";
	db.doQuery(queryString.str());
	//std::cout << "X: " << js->lX << " Y: " << js->lY << " Z: " << js->lZ << " " << js->rglSlider[0] << " " << js->rglSlider[1]
	//	<< " " << js->lRx << " " << js->lRy << " " << js->lRz << std::endl;
}

void DataManager::join()
{
	std::cout << "TERMINANDO" << std::endl;
	timeToQuit = false;
}

DataManager::~DataManager()
{
}
