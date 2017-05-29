#include "stdafx.h"
#include "PostgreSQL.h"
#include <iostream> //Should return messages but it easier
#include <sstream>





bool PostgreSQL::connect()
{
	/*std::stringstream ss;
	ss << "dbname = " << this->dbname;
	ss << " user = " << this->user;
	ss << " password = " << this->password << " hostaddr = ";
	ss << this->hostaddr << " port = " << this->port;*/
	dbconn = PQconnectdb("dbname = postgres user = postgres password = 1way.Street hostaddr = 127.0.0.1 port = 5432");
	if (PQstatus(dbconn) == CONNECTION_BAD) {
		std::cout << "Unable to connect to database\n" << std::endl;
		return false;
	}
	return true;
}

bool PostgreSQL::doQuery(std::string & queryString)
{
	return this->doQuery(queryString.c_str());
}

bool PostgreSQL::doQuery(const char * queryString)
{
	this->query = PQexec(dbconn, queryString);
	if (PQresultStatus(this->query) != PGRES_COMMAND_OK) {
		endQueryError();
		return false;
	}

	PQclear(this->query);
	return true;
}

bool PostgreSQL::doQuerySelect(const char * queryString)
{
	this->query = PQexec(dbconn, queryString);
	if (PQresultStatus(this->query) != PGRES_TUPLES_OK) {
		endQueryError();
		return false;
	}
	return true;
}

void PostgreSQL::endQueryError()
{
	//std::cerr << "ERROR: " << PQerrorMessage(this->dbconn) << std::endl;
	PQclear(this->query);
	PQfinish(dbconn);
	this->finish = true;
}

std::string PostgreSQL::getValue(int tup_num, int field_num)
{
	return std::string(PQgetvalue(this->query, tup_num, field_num));
}

PostgreSQL::~PostgreSQL()
{
	if (!finish) {
		PQfinish(this->dbconn);
	}
}
