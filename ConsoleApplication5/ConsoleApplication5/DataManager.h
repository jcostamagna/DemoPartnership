#pragma once
#include <atomic>
#include "Joystick.h"

class DataManager
{
private:
	int idSession;
	std::atomic<bool> timeToQuit;
	DataManager(const DataManager& object);
	DataManager& operator=(const DataManager& object);
	Joystick joy;
public:
	DataManager(int id) : idSession(id) {
		timeToQuit = false;
	}
	void startCollectingData();
	void join();
	~DataManager();
};

