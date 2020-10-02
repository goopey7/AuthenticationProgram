//
// Created by sam on 01/10/2020.
//

#ifndef AUTHENTICATIONPROGRAM_COOKIEUI_H
#define AUTHENTICATIONPROGRAM_COOKIEUI_H


#include "MenuInterface.h"
#include "GameInstance.h"

class CookieUI : MenuInterface
{
private:
	GameInstance* instance;
public:
	CookieUI(GameInstance* _instance);
	void display() override;
	bool userInteraction(std::string &choice) override;
	int mapInputToEnum(std::string &choice);
	enum Options
	{
		Logout,
		Click,
		PurchaseGrandmother,
		PurchaseGreatGrandmother,
		PurchaseSuperOven,
		FindFriend,
		INVALID
	};
};


#endif //AUTHENTICATIONPROGRAM_COOKIEUI_H
