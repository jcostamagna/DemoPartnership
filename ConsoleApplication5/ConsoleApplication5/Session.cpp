#include "stdafx.h"
#include "Session.h"
#include <iostream>
#include <sstream>
#include <libpq-fe.h>
#include <thread>



void Session::startCollectingData(int idS)
{
	std::cout << "SESION  ID" << idS << std::endl;
	//threadData = std::thread (&DataManager::startCollectingData, this->dataManager);
	this->dataManager.startCollectingData(idS);
}

void Session::endCollectingData()
{
	this->dataManager.join();
	threadData.join();

}

void Session::start()
{
	startDataBase();
	threadData = std::thread(&Session::startCollectingData, this, this->id);
	//startCollectingData();
	std::cout << "The sesion " << id << " starts fine!" << std::endl;
}

void Session::end()
{
	endCollectingData();
	endDataBase();
	std::cout << "The sesion " << id << " ends!" << std::endl;
}


Session::~Session()
{
}


void Session::startDataBase()
{
	PGconn *dbconn;
	dbconn = PQconnectdb("dbname = postgres user = postgres password = 1way.Street hostaddr = 127.0.0.1 port = 5432");
	if (PQstatus(dbconn) == CONNECTION_BAD) {
		std::cout << "Unable to connect to database\n" << std::endl;
		return;
	}




	PGresult* query = PQexec(dbconn, "CREATE TABLE IF NOT EXISTS Session(Id INTEGER PRIMARY KEY, Start INT, Finish INT)");

	if (PQresultStatus(query) != PGRES_COMMAND_OK) {
		std::cout << PQerrorMessage(dbconn);
		PQclear(query);
		PQfinish(dbconn);
		return;
	}

	PQclear(query);
	std::stringstream queryString;
	time(&this->timer);
	queryString << "INSERT INTO Session VALUES(" << this->id << "," << this->timer << ",NULL)";

	query = PQexec(dbconn, queryString.str().c_str());

	if (PQresultStatus(query) != PGRES_COMMAND_OK) {
		std::cout << PQerrorMessage(dbconn);
		PQclear(query);
		PQfinish(dbconn);
		return;
	}

	PQclear(query);
	PQfinish(dbconn);
}

void Session::endDataBase()
{
	PGconn *conn;
	conn = PQconnectdb("dbname = postgres user = postgres password = 1way.Street hostaddr = 127.0.0.1 port = 5432");
	if (PQstatus(conn) == CONNECTION_BAD) {
		std::cout << "Unable to connect to database\n" << std::endl;
		return;
	}

	PGresult *query;

	std::stringstream queryString;
	time(&this->timer);
	queryString << "UPDATE Session SET Finish = " << this->timer << " WHERE Id = " << this->id;
	query = PQexec(conn, queryString.str().c_str());

	PQfinish(conn);
}
