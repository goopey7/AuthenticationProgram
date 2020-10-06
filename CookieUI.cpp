//
// Created by sam on 01/10/2020.
//

#include "CookieUI.h"

CookieUI::CookieUI(GameInstance* _instance)
{
	instance=_instance;
}

void CookieUI::display()
{
	std::cout << "********************************************************************************"<<std::endl;
	std::cout << "*                                  Cookie Clicker                              *"<<std::endl;
	std::cout << "*                                   Sam Collier                                *"<<std::endl;
	std::cout << "********************************************************************************"<<std::endl;
	if(instance->isGameMaster())std::cout<<"Welcome Game Master!\n";
	std::cout << "********* Cookies: " << instance->getNumCookies() << std::endl;
	std::cout << "********* Cookies per second: "<< instance->getCookieRate() << std::endl;
	std::cout << "********* Cookies per click: "<< instance->getCookieClickRate()<<std::endl;
	if(instance->getFollowing())
	{
		for(std::string user : *instance->getFollowing())
			std::cout << user << ": " << instance->getNumCookies(user) << " cookies\n";
	}

	std::cout << "Please select one of the following options:\n";
	std::cout << "1) Click\n";
	std::cout << "2) Purchase Grandmother - Add 2 Cookies/Sec - 8 Cookies\n";
	std::cout << "3) Purchase Great Grandmother - Add 4 Cookies/Sec - 200 cookies\n";
	std::cout << "4) Purchase Super Oven - Add 999 Cookies/Sec - 10000000 cookies\n";
	std::cout << "5) Find User\n";
	std::cout << "6) Send Cookies\n";
	std::cout << "7) Refresh\n";
	if(instance->isGameMaster())
	{
		std::cout << "8) Set cookies for user\n";
	}
	std::cout << "0) Logout\n";
}

bool CookieUI::userInteraction(std::string &choice)
{
	ReadAndWrite::getInputAsString(choice);
	return false;
}

int CookieUI::mapInputToEnum(std::string &choice)
{
	int numChoice=-1;
	if(choice=="0"||choice=="1"||choice=="2"||choice=="3"||choice=="4"||choice=="5"||choice=="6"||
	choice=="7"||(instance->isGameMaster()&&choice=="8"))
		numChoice=std::stoi(choice);
	return numChoice;
}
