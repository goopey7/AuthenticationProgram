//
// Created by sam on 03/10/2020.
//

#include "FindUserUI.h"

void FindUserUI::display()
{
	std::cout << "********************************************************************************"<<std::endl;
	std::cout << "*                                  Cookie Clicker                              *"<<std::endl;
	std::cout << "*                                   Sam Collier                                *"<<std::endl;
	std::cout << "********************************************************************************"<<std::endl;
	std::cout << "Select who you'd like to follow: \n";
	for(std::string user : *instance->)
		std::cout << user << ": " << instance->getNumCookies(user) << " cookies\n";
}

bool FindUserUI::userInteraction(std::string &choice)
{
	return false;
}
