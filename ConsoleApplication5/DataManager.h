#pragma once
#include <atomic>
#include <time.h>
#include <vector>
#include "Joystick.h"

class DataManager
{
private:
	int idSession;
	std::atomic<bool> timeToQuit;
	DataManager(const DataManager& object);
	DataManager& operator=(const DataManager& object);
	Joystick joy;
	time_t timer;
public:
	DataManager(int id) {
		this->idSession = id;
		timeToQuit = false;
	}
	void startCollectingData(int id);
	void initializeData();
	void saveData(int id, DIJOYSTATE* js, std::vector<double>& vrData);
	void join();
	~DataManager();
};

