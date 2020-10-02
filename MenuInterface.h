//
// Created by sam on 29/09/2020.
//

#ifndef AUTHENTICATIONPROGRAM_MENUINTERFACE_H
#define AUTHENTICATIONPROGRAM_MENUINTERFACE_H
#include <iostream>
#include "ReadAndWrite.h"

class MenuInterface
{
public:

	virtual void display()=0; // fully abstract

	// if the user input is bad, return true. If it's good, return false.
	virtual bool userInteraction(std::string &choice)=0;

};


#endif //AUTHENTICATIONPROGRAM_MENUINTERFACE_H
