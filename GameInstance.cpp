//
// Created by sam on 01/10/2020.
//

#include "GameInstance.h"

GameInstance::GameInstance(int _accIndex,std::vector<std::string>* database)
{
	accIndex=_accIndex;
	if(database->at(accIndex+3)=="}")
	{
		numCookies=0;
		database->insert(database->begin()+accIndex+3,"cookies:"+std::to_string(numCookies));
	}
	else
	{
		numCookies=std::stoi(database->at(accIndex+3).substr(8));
		std::cout << "Num Cookies: " << numCookies;
	}
}
