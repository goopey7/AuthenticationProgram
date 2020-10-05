//
// Created by sam on 01/10/2020.
//

#include <sstream>
#include "GameInstance.h"

GameInstance::GameInstance(int _accIndex,std::vector<std::string>* _database)
{
	accIndex=_accIndex;
	database=_database;
	users = new std::vector<std::string>;
	// if there is no cookie entry in the database
	if(database->at(accIndex+NUM_COOKIE_OFFSET)=="}")
	{
		numCookies=0;

		//insert a database entry for cookies on the line between the account password and the line afterwards
		database->insert(database->begin()+accIndex+NUM_COOKIE_OFFSET,"cookies:"+std::to_string(numCookies));
	}
	// parse the cookie entry into numCookies
	else
		numCookies=std::stod(database->at(accIndex+NUM_COOKIE_OFFSET).substr(8));

	// if there is no cookie rate stored in the database
	if(database->at(accIndex+COOKIE_RATE_OFFSET)=="}")
	{
		cookieRate=0.f;

		//insert a database entry for cookieRate on the line between the numCookies and the line afterwards
		database->insert(database->begin()+accIndex+COOKIE_RATE_OFFSET,"rate:"+std::to_string(cookieRate));
	}
	// parse the rate entry into cookieRate
	else
		cookieRate=std::stof(database->at(accIndex+COOKIE_RATE_OFFSET).substr(5));

	// if there is no click rate stored in the database
	if(database->at(accIndex+CLICK_RATE_OFFSET)=="}")
	{
		clickRate=1;

		//insert a database entry for cookieRate on the line between the numCookies and the "}"
		database->insert(database->begin()+accIndex+CLICK_RATE_OFFSET,"click:"+std::to_string(clickRate));
	}
	// parse the click entry into clickRate
	else
		clickRate=std::stoi(database->at(accIndex+CLICK_RATE_OFFSET).substr(6));

	// if there is no following entry stored in the database
	following=new std::vector<std::string>;
	if(database->at(accIndex+FOLLOWING_OFFSET)=="}")
	{
		//insert a database entry for cookieRate on the line between the numCookies and the "}"
		database->insert(database->begin()+accIndex+FOLLOWING_OFFSET,"follows:");
	}
	// parse through each user we are following separated by comma. TODO: Don't allow usernames to have commas
	else
	{
		refreshFollowingList();
	}

	for(std::string line : *database)
	{
		if(line.find("ID:")!=std::string::npos)
			users->push_back(line.substr(3));
	}
}

inline void GameInstance::refreshFollowingList()
{
	following=new std::vector<std::string>;
	std::string users= database->at(accIndex + FOLLOWING_OFFSET).substr(8);
	std::stringstream sStream(users);
	while(sStream.good()&&users.length()>0)
	{
		std::string subStr;
		std::getline(sStream,subStr,',');
		following->push_back(subStr);
	}
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

std::vector<std::string>* GameInstance::getFollowing()
{
	return following;
}

double GameInstance::getNumCookies(std::string user)
{
	int i=0;
	for(std::string line : *database)
	{
		if(line=="ID:"+user)
			return std::stod(database->at(i+NUM_COOKIE_OFFSET).substr(8));;
		i++;
	}
	return -1;
}

std::vector<int>* GameInstance::getAccIndices()
{
	std::vector<int>* out=new std::vector<int>;
	int i=0;
	for(std::string line : *database)
	{
		if(line.find("ID:")!=std::string::npos)
		{
			out->push_back(i);
		}
		i++;
	}
	return out;
}

std::string GameInstance::getUserName(int index)
{
	return database->at(index).substr(3);
}

int GameInstance::getAccIndex()
{
	return accIndex;
}

void GameInstance::follow(std::string &choice,std::vector<std::string>* choices)
{
	try
	{
		int index = std::stoi(choice)-1;
		std::string followLine=database->at(accIndex + FOLLOWING_OFFSET);
		if(followLine.at(followLine.size()-1)==':')
			followLine+=choices->at(index);
		else
			followLine+=","+choices->at(index);
		database->at(accIndex+FOLLOWING_OFFSET)=followLine;
		saveChanges();
		refreshFollowingList();
	}
	catch(...)
	{

	}

}

