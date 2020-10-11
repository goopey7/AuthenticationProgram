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
	// if there is more than one account in the database
	if(instance->getAccIndices()->size()>1)numChoices=1;
	choices=new std::vector<std::string>;
	// for every user in the database
	for(int accIndex : *instance->getAccIndices())
	{
		bool bAlreadyFollows=false;
		std::string user = instance->getUserName(accIndex);
		for(std::string followedUser : *instance->getFollowing())
			if(user==followedUser)bAlreadyFollows=true;
		// if the current index is not us and we don't already follow it, display the account
		if(accIndex!=instance->getAccIndex()&&!bAlreadyFollows)
		{
			std::cout << numChoices << ": ";
			std::cout << user << std::endl;
			numChoices++;
			choices->push_back(user);
		}
	}
	// the for loop would have incremented numChoices after displaying the last element, so we must decrement it.
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
		// if we weren't able to get an integer out of the input see if the user was trying to exit.
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
