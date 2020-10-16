//
// Created by sam on 03/10/2020.
//

#ifndef AUTHENTICATIONPROGRAM_FINDUSERUI_H
#define AUTHENTICATIONPROGRAM_FINDUSERUI_H

#include "MenuInterface.h"
#include "GameInstance.h"

class FindUserUI : MenuInterface
{
private:
	class GameInstance* instance;
	std::vector<std::string>* choices;
	int numChoices=0;
public:
	FindUserUI(GameInstance* _instance);
	void display() override;
	bool userInteraction(std::string &choice) override;
	std::vector<std::string>* getChoices();
};


#endif //AUTHENTICATIONPROGRAM_FINDUSERUI_H
