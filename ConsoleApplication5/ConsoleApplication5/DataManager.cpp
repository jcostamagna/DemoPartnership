#include "stdafx.h"
#include "DataManager.h"
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <chrono>
#include <string>
#include "PostgreSQL.h"

void DataManager::startCollectingData(int id)
{
	this->idSession = id;
	initializeData();
	joy.start();
	while (this->timeToQuit.load()) {
		//std::cout << "SESION:" << id << std::endl;
		saveData(id, joy.getJoy());
		Sleep(100);
	}
	joy.close();
	//std::cout << "TERMINE" << std::endl;
}

void DataManager::initializeData()
{
	PostgreSQL db;
	if (!db.connect()) { return;}
	bool succes = db.doQuery("CREATE TABLE IF NOT EXISTS Data(time timestamp, IDSESSION INTEGER REFERENCES Session(Id), DeviceType VARCHAR(40), STEERING INT, ACCELERATOR INT, BRAKE INT"\
		", SLIDER1 INT, PRIMARY KEY (time, IDSESSION) );");

	if (!succes) { return; }
}

void DataManager::saveData(int id, DIJOYSTATE * js)
{
	PostgreSQL db;
	db.connect();
	std::stringstream queryString;
	time(&this->timer);

	SYSTEMTIME time;
	GetSystemTime(&time);


	queryString << "INSERT INTO Data VALUES( to_timestamp(" << this->timer << time.wMilliseconds << "::double precision / 1000)," << id << ", " << "'Steering'" << 
		"," << js->lX << "," << js->lY << "," << js->rglSlider[0] << "," << js->lRz <<")";
	db.doQuery(queryString.str());
	//std::cout << "X: " << js->lX << " Y: " << js->lY << " Z: " << js->lZ << " " << js->rglSlider[0] << " " << js->rglSlider[1]
		//<< " " << js->lRx << " " << js->lRy << " " << js->lRz << std::endl;
}

void DataManager::join()
{
	//std::cout << "TERMINANDO" << std::endl;
	timeToQuit = false;
}

DataManager::~DataManager()
{
}
