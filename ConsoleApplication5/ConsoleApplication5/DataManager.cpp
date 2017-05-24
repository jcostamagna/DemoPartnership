#include "stdafx.h"
#include "DataManager.h"
#include <iostream>
#include <Windows.h>


void DataManager::startCollectingData()
{
	while (this->timeToQuit.load()) {
		std::cout << "Estoy corriendo" << std::endl;
		joy.start();
		Sleep(100);
	}
	std::cout << "TERMINE" << std::endl;
}

void DataManager::join()
{
	std::cout << "TERMINANDO" << std::endl;
	timeToQuit = false;
}

DataManager::~DataManager()
{
}
