//
// Created by sam on 01/10/2020.
//

#include "GameInstance.h"

GameInstance::GameInstance(int _accIndex,std::vector<std::string>* _database)
{
	accIndex=_accIndex;
	database=_database;
	// if there is no cookie entry in the database
	if(database->at(accIndex+3)=="}")
	{
		numCookies=0;

		//insert a database entry for cookies on the line between the account password and the line afterwards
		database->insert(database->begin()+accIndex+3,"cookies:"+std::to_string(numCookies));
	}
	// parse the cookie entry into numCookies
	else
		numCookies=std::stoi(database->at(accIndex+3).substr(8));

	// if there is no cookie rate stored in the database
	if(database->at(accIndex+4)=="}")
	{
		cookieRate=0.f;

		//insert a database entry for cookieRate on the line between the numCookies and the line afterwards
		database->insert(database->begin()+accIndex+4,"rate:"+std::to_string(cookieRate));
	}
	// parse the rate entry into cookieRate
	else
		cookieRate=std::stof(database->at(accIndex+4).substr(5));

	// if there is no click rate stored in the database
	if(database->at(accIndex+5)=="}")
	{
		clickRate=1;

		//insert a database entry for cookieRate on the line between the numCookies and the "}"
		database->insert(database->begin()+accIndex+5,"click:"+std::to_string(clickRate));
	}
	// parse the click entry into clickRate
	else
		clickRate=std::stoi(database->at(accIndex+5).substr(6));
}

void GameInstance::addCookie(int amountToAdd)
{
	if(amountToAdd>0)
		numCookies+=amountToAdd;
	saveChanges();
}

bool GameInstance::subtractCookie(int amountToSubtract)
{
	if(amountToSubtract>0&&numCookies-amountToSubtract>=0)
	{
		numCookies-=amountToSubtract;
		saveChanges();
		return true;
	}
	return false;
}

void GameInstance::addToRate(float amountToAdd)
{
	cookieRate+=amountToAdd;
	saveChanges();
}

void GameInstance::saveChanges()
{
	database->at(accIndex+3)="cookies:"+std::to_string(numCookies);
	database->at(accIndex+4)="rate:"+std::to_string(cookieRate);
	ReadAndWrite::writeFile(database,"database.txt");
}

double GameInstance::getNumCookies()
{
	return numCookies;
}

int GameInstance::getCookieRate()
{
	return cookieRate;
}

int GameInstance::getCookieClickRate()
{
	return clickRate;
}

bool GameInstance::destroyed()
{
	return bDestroyed;
}
