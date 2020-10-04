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
	GameInstance* instance;
public:
	void display() override;
	bool userInteraction(std::string &choice) override;
};


#endif //AUTHENTICATIONPROGRAM_FINDUSERUI_H
