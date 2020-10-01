//
// Created by sam on 30/09/2020.
//

#include "MainMenu.h"

void MainMenu::display()
{
		std::cout << "********************************************************************************"<<std::endl;
		std::cout << "*                                  Cookie Clicker                              *"<<std::endl;
		std::cout << "*                                   Sam Collier                                *"<<std::endl;
		std::cout << "********************************************************************************"<<std::endl;
		std::cout << "Please select one of the following options: \n";
		std::cout << "1) Login"<<std::endl;
		std::cout << "2) Create a new account"<<std::endl;
}

bool MainMenu::userInteraction()
{
	std::string userInput;
	ReadAndWrite::getInputAsString(userInput);
	if(userInput!="1"&&userInput!="2")return true;
	return false;
}
