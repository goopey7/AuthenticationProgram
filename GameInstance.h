//
// Created by sam on 01/10/2020.
//

#ifndef AUTHENTICATIONPROGRAM_GAMEINSTANCE_H
#define AUTHENTICATIONPROGRAM_GAMEINSTANCE_H
#include <iostream>
#include <vector>

class GameInstance
{
private:
	int accIndex;
	int numCookies;
public:
	GameInstance(int _accIndex,std::vector<std::string>* database);
	void addCookie(int amountToAdd);
	void subtractCookie(int amountToSubtract);
};

#endif //AUTHENTICATIONPROGRAM_GAMEINSTANCE_H
