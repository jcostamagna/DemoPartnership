#include "stdafx.h"
#include "DataManager.h"
#include <iostream>
#include <sstream>
#include <Windows.h>
#include <chrono>
#include <string>
#include "PostgreSQL.h"
#include "HTCViveVR.h"

void DataManager::startCollectingData(int id)
{
	LighthouseTracking vr;
	initializeData();
	joy.start();
	while (this->timeToQuit.load()) {
		//std::cout << "SESION:" << id << std::endl;
		saveData(id, joy.getJoy(), vr.ParseTrackingFrame());
		Sleep(100);
	}
	joy.close();
	//std::cout << "TERMINE" << std::endl;
}

void DataManager::initializeData()
{
	PostgreSQL db;
	if (!db.connect()) { return;}
	bool succes = db.doQuery("CREATE TABLE IF NOT EXISTS DataSteering(time timestamp, IDSESSION INTEGER REFERENCES Session(Id), STEERING INT, ACCELERATOR INT, BRAKE INT"\
		", SLIDER1 INT, PRIMARY KEY (time, IDSESSION) );");

	if (!succes) { return; }

	succes = db.doQuery("CREATE TABLE IF NOT EXISTS DataVr(time timestamp, IDSESSION INTEGER REFERENCES Session(Id),  PositionX FLOAT, PositionY FLOAT, PositionZ FLOAT"\
		", RotationX FLOAT, RotationY FLOAT, RotationZ FLOAT, PRIMARY KEY (time, IDSESSION) );");

	if (!succes) { return; }
}

void DataManager::saveData(int id, DIJOYSTATE * js, std::vector<double>& vrData)
{
	PostgreSQL db;
	db.connect();
	std::stringstream queryString;
	time(&this->timer);

	SYSTEMTIME time;
	GetSystemTime(&time);


	queryString << "INSERT INTO DataSteering VALUES( to_timestamp(" << this->timer << std::setw(3) << std::setfill('0') << time.wMilliseconds << "::double precision / 1000)," << id <<
		"," << js->lX << "," << js->lY << "," << js->rglSlider[0] << "," << js->lRz <<")";
	db.doQuery(queryString.str());

	queryString.str("");

	queryString << "INSERT INTO DataVr VALUES( to_timestamp(" << this->timer << std::setw(3) << std::setfill('0') << time.wMilliseconds << "::double precision / 1000)," << id <<
		"," << vrData.at(0) << "," << vrData.at(1) << "," << vrData.at(2) << "," << vrData.at(3) << "," << vrData.at(4) << "," << vrData.at(5) << ")";
	db.doQuery(queryString.str());


}

void DataManager::join()
{
	//std::cout << "TERMINANDO" << std::endl;
	timeToQuit = false;
}

DataManager::~DataManager()
{
}
