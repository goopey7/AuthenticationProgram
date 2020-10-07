//
// Created by sam on 01/10/2020.
//

#include <sstream>
#include "GameInstance.h"

GameInstance::GameInstance(int _accIndex,std::vector<std::string>* _database,bool _bIsGameMaster,std::string _nickname)
{
	accIndex=_accIndex;
	database=_database;
	refreshDatabase(_bIsGameMaster,_nickname);
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

void GameInstance::addCookie(double amountToAdd)
{
	if(amountToAdd>0)
		numCookies+=amountToAdd;
	//update database
	database->at(accIndex+NUM_COOKIE_OFFSET)="cookies:"+std::to_string(numCookies);
	saveChanges();
}

/*
 * returns whether or not we successfully took away the specified cookies
 */
bool GameInstance::subtractCookie(double amountToSubtract)
{
	// if we don't have enough cookies for the transaction
	if(amountToSubtract>0&&numCookies-amountToSubtract>=0)
	{
		numCookies-=amountToSubtract;
		database->at(accIndex+NUM_COOKIE_OFFSET)="cookies:"+std::to_string(numCookies);
		saveChanges();
		return true;
	}
	return false;
}

void GameInstance::addToRate(double amountToAdd)
{
	cookieRate+=amountToAdd;
	//update database
	database->at(accIndex+COOKIE_RATE_OFFSET)="rate:"+std::to_string(cookieRate);
	saveChanges();
}

void GameInstance::saveChanges()
{
	// Commit changes from memory to the file.
	ReadAndWrite::writeFile(database,"database.txt");
}

double GameInstance::getNumCookies()
{
	return numCookies;
}

double GameInstance::getCookieRate()
{
	return cookieRate;
}

double GameInstance::getCookieClickRate()
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
		if(line=="ID:"+user) // once we get to the specified user's entry
			//read the amount of cookies they have
			return std::stod(database->at(i+NUM_COOKIE_OFFSET).substr(8));
		i++;
	}
	return -1;
}

/*
 * Get the index of every account in the database
 */
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
	// read the database and take off the "ID:" part
	return database->at(index).substr(3);
}

int GameInstance::getAccIndex()
{
	return accIndex;
}

/*
 * Follow a specific player
 */
void GameInstance::follow(std::string &choice,std::vector<std::string>* choices)
{
	try
	{
		// the choice should be an int
		int index = std::stoi(choice)-1; // subtract one to get the correct index
		std::string followLine=database->at(accIndex + FOLLOWING_OFFSET); //acquire our follow line
		if(followLine.at(followLine.size()-1)==':')
			followLine+=choices->at(index); // add our choice
		else
			followLine+=","+choices->at(index); // add our choice after previous username
		database->at(accIndex+FOLLOWING_OFFSET)=followLine; // modify our follow line
		saveChanges();
		refreshFollowingList();
	}
	catch(...)
	{

	}

}

bool GameInstance::isGameMaster()
{
	return bIsGameMaster;
}
/*
 * Sets the specified account's balance to the specified amount
 */
void GameInstance::setCookies(std::string user,double amountToSet)
{
	int i=0;
	for(std::string line : *database)
	{
		if(line=="ID:"+user)
		{
			database->at(i+NUM_COOKIE_OFFSET)="cookies:"+std::to_string(amountToSet);
		}
		i++;
	}
	saveChanges();
	refreshDatabase(bIsGameMaster,"");
}

void GameInstance::refreshDatabase(bool _bIsGameMaster,std::string _nickname)
{
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

	// if there is no permission entry stored in the database
	if(database->at(accIndex+PERMISSION_OFFSET)=="}")
	{
		bIsGameMaster=_bIsGameMaster;
		//insert a database entry for cookieRate on the line between the numCookies and the "}"
		database->insert(database->begin()+accIndex+PERMISSION_OFFSET,"master:"
																	  +std::to_string(bIsGameMaster));
	}
		// parse master status into boolean
	else
	{
		bIsGameMaster=std::stoi(database->at(accIndex+PERMISSION_OFFSET).substr(7));
	}

	// if there is no nickname entry stored in the database
	if(database->at(accIndex+NICKNAME_OFFSET)=="}")
	{
		//insert a database entry of the nickname
		database->insert(database->begin()+accIndex+NICKNAME_OFFSET,"nickname:"+_nickname);
		nickname=_nickname;
	}
	// parse the nickname from the database entry
	else
	{
		nickname=database->at(accIndex+NICKNAME_OFFSET).substr(9);
	}
}

std::string GameInstance::getNickName()
{
	return nickname;
}

