//
// Created by sam on 30/09/2020.
//

#ifndef AUTHENTICATIONPROGRAM_MAINMENU_H
#define AUTHENTICATIONPROGRAM_MAINMENU_H

#include "ReadAndWrite.h"
#include "MenuInterface.h"

class MainMenu : MenuInterface
{
public:
	void display() override;
	bool userInteraction() override;
};


#endif //AUTHENTICATIONPROGRAM_MAINMENU_H
