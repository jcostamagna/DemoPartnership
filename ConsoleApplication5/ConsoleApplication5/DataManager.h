#pragma once
#include <atomic>

class DataManager
{
private:
	int idSession;
	std::atomic<bool> timeToQuit;
	DataManager(const DataManager& object);
	DataManager& operator=(const DataManager& object);
public:
	DataManager(int id) : idSession(id) {
		timeToQuit = false;
	}
	void startCollectingData();
	void join();
	~DataManager();
};

