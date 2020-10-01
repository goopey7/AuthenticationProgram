//
// Created by sam on 29/09/2020.
//

#include "MainMenu.h"

int main()
{
	MainMenu* mainMenu=new MainMenu;
	do
	{
		mainMenu->display();
	}
	while (mainMenu->userInteraction());
	return 0;
}
