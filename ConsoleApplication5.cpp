// ConsoleApplication5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "Session.h"
#include "PostgreSQL.h"


int getLastId() {
	PostgreSQL db;
	if (!db.connect()) return 0;
	
	if (!db.doQuerySelect("SELECT MAX(Id) FROM Session")) {return 0;}
	int id = std::stoi(db.getValue(0, 0));
	return ++id;
}



int main()
{
	std::cout << "Press 'q' for exit or any key to continue" << std::endl;
	std::string input = "";
	std::cin >> input;
	int idSession = getLastId();
	Session* session;
	std::cout << "Session Id: " << idSession << std::endl;
	while (input != "q") {
		session = new Session(idSession);
		session->start();
		std::cout << "Press 'q' to exit or 's' to stop the session" << std::endl;
		std::cin >> input;
		while (input != "q" && input != "s") {
			std::cout << "Press 'q' to exit or 's' to stop the session" << std::endl;
			std::cin >> input;
		}
		session->end();
		delete session;
		idSession++;
		if (input != "q") { 
			std::cout << "Press 'q' for exit or any key to continue" << std::endl;
			std::cin >> input; 
		}
	}

    return 0;
}

