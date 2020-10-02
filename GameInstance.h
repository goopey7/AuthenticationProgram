//
// Created by sam on 01/10/2020.
//

#ifndef AUTHENTICATIONPROGRAM_GAMEINSTANCE_H
#define AUTHENTICATIONPROGRAM_GAMEINSTANCE_H
#include <iostream>
#include <vector>
#include "ReadAndWrite.h"

class GameInstance
{
private:
	int accIndex;
	std::vector<std::string>* database;
	double numCookies;
	float cookieRate;
	int clickRate;
public:
	bool bDestroyed;
	GameInstance(int _accIndex,std::vector<std::string>* _database);
	void addCookie(int amountToAdd);
	bool subtractCookie(int amountToSubtract);
	void saveChanges();
	void addToRate(float amountToAdd);
	double getNumCookies();
	int getCookieRate();
	int getCookieClickRate();
	bool destroyed();
};

#endif //AUTHENTICATIONPROGRAM_GAMEINSTANCE_H
