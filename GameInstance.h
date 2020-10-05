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
	std::vector<std::string>* following;
	const int NUM_COOKIE_OFFSET=3;
	const int COOKIE_RATE_OFFSET=4;
	const int CLICK_RATE_OFFSET=5;
	const int FOLLOWING_OFFSET=6;
	std::vector<std::string>* users;
public:
	bool bDestroyed;
	GameInstance(int _accIndex,std::vector<std::string>* _database);
	void addCookie(int amountToAdd);
	bool subtractCookie(int amountToSubtract);
	void saveChanges();
	void addToRate(float amountToAdd);
	double getNumCookies(); // get cookies of logged in user
	double getNumCookies(std::string user);
	int getCookieRate();
	int getCookieClickRate();
	std::vector<std::string>* getFollowing();
	std::vector<int>* getAccIndices();
	bool destroyed();
	std::string getUserName(int index);
	int getAccIndex();
	void follow(std::string &choice,std::vector<std::string>* choices);

	void refreshFollowingList();
};

#endif //AUTHENTICATIONPROGRAM_GAMEINSTANCE_H
