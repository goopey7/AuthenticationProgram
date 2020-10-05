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
	std::cout << "Press E to exit\n";
	if(instance->getAccIndices()->size()>1)numChoices=1;
	choices=new std::vector<std::string>;
	for(int accIndex : *instance->getAccIndices())
	{
		bool bAlreadyFollows=false;
		std::string user = instance->getUserName(accIndex);
		for(std::string followedUser : *instance->getFollowing())
			if(user==followedUser)bAlreadyFollows=true;
		if(accIndex!=instance->getAccIndex()&&!bAlreadyFollows)
		{
			std::cout << numChoices << ": ";
			std::cout << user << std::endl;
			numChoices++;
			choices->push_back(user);
		}
	}
	if(numChoices>0)numChoices--;
}

bool FindUserUI::userInteraction(std::string &choice)
{
	if(numChoices<=0)return false;
	try
	{
		ReadAndWrite::getInputAsString(choice);
		if(std::stoi(choice)>numChoices||std::stoi(choice)<1)
			return true;
	}
	catch (...)
	{
		if(choice=="e"||choice=="E")
			return false;
		return true;
	}
	return false;
}

FindUserUI::FindUserUI(GameInstance* _instance)
{
	instance=_instance;
}

std::vector<std::string>* FindUserUI::getChoices()
{
	return choices;
}
