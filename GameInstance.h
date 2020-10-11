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
	const int PERMISSION_OFFSET=7;
	const int NICKNAME_OFFSET=8;
	bool bIsGameMaster;
	std::string nickname;
public:
	bool bDestroyed;
	GameInstance(int _accIndex,std::vector<std::string>* _database,bool _bIsGameMaster=false,std::string _nickname="");
	void addCookie(double amountToAdd);
	bool subtractCookie(double amountToSubtract);
	void saveChanges();
	void addToRate(double amountToAdd);
	double getNumCookies(); // get cookies of logged in user
	double getNumCookies(std::string user);
	double getCookieRate();
	double getCookieClickRate();
	std::vector<std::string>* getFollowing();
	std::vector<int>* getAccIndices();
	bool destroyed();
	std::string getUserName(int index);
	int getAccIndex();
	void follow(std::string &choice,std::vector<std::string>* choices);
	void refreshFollowingList();
	bool isGameMaster();
	void setCookies(std::string user,double amountToSet);
	void refreshDatabase(bool _bIsGameMaster=false,std::string _nickname="");
	std::string getNickName();
};

#endif //AUTHENTICATIONPROGRAM_GAMEINSTANCE_H
