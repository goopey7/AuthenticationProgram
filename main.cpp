//
// Created by sam on 29/09/2020.
//

#include <future>
#include "MainMenu.h"
#include "sha256.h"
#include "GameInstance.h"
#include "CookieUI.h"
#include "FindUserUI.h"

#ifdef _WIN32_
#include <Windows.h>
#define CLEAR_SCREEN system("cls");
#else
#include <unistd.h>
#define CLEAR_SCREEN system("clear");
#endif

void applyCookiePerSecond(GameInstance* instance)
{
	while(!instance->destroyed())
	{
		if(instance->getCookieRate()>0)
			instance->addCookie(instance->getCookieRate());
		sleep(1);
	}
}

inline uint64_t getNanosSinceEpoch()
{
	std::chrono::time_point<std::chrono::system_clock> ts = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(ts.time_since_epoch()).count();
}

int main()
{
	MainMenu* mainMenu=new MainMenu;
	std::string choice;
	do
	{
		choice="";
		mainMenu->display();
	}
	while (mainMenu->userInteraction(choice));
	std::vector<std::string>* database = ReadAndWrite::readFile("database.txt");
	GameInstance* instance;
	if(choice=="1")
	{
		//Login
		bool bLoginFail=true;
		do
		{
			CLEAR_SCREEN
			std::string username;
			std::cout << "Enter username: ";
			ReadAndWrite::getInputAsString(username);
			std::string password;
			std::cout << "Enter password: ";
			ReadAndWrite::getInputAsString(password);
			int accIndex=-1;
			for(int i=0;i<database->size();i++)
			{
				if("ID:"+username==database->at(i))
				{
					accIndex=i;
					break;
				}
			}
			if(accIndex!=-1)
			{
				std::string hashLine=database->at(accIndex+2);

				// Extract the hash and the salt from the database entry
				std::string hash="";
				std::string salt="";
				for(int i=0;i<hashLine.length();i++)
				{
					if(i%2==0)
						hash+=hashLine.at(i);
					else
						salt+=hashLine.at(i);
				}
				// verify password
				if(hash==picosha2::hash256_hex_string(password+salt))
				{
					bLoginFail=false;
					std::cout << "*** Login Success ***\n";
					instance = new GameInstance(accIndex,database,false);
				}
			}
			else
				std::cout << "ERROR: Invalid Login\n";
		}
		while(bLoginFail);
	}
	else if(choice=="2")
	{
		//Create account
		std::string newAccountName;

		// Ask for account name and check for duplicates
		bool bDuplicateAccName=false;
		do
		{
			CLEAR_SCREEN
			bDuplicateAccName=false;
			std::cout << "Enter in a username for this account: ";
			ReadAndWrite::getInputAsString(newAccountName);
			for(std::string s : *database)
			{
				if(s=="ID:"+newAccountName)
				{
					bDuplicateAccName = true;
					std::cout << "ERROR: Account Name Already Exists\n";
				}
			}
		}
		while (bDuplicateAccName);
		newAccountName = "ID:"+newAccountName;
		int accIndex = database->size();
		database->push_back(newAccountName);
		database->push_back("{");
		// Ask for a password
		std::cout << "Enter in a password for this account: ";
		std::string clearPassword;
		ReadAndWrite::getInputAsString(clearPassword);

		// Confirm password
		std::string confirmedPass;
		do
		{
			std::cout << "Confirm password: ";
			ReadAndWrite::getInputAsString(confirmedPass);
			if(confirmedPass!=clearPassword)
				std::cout << "ERROR: Passwords do not match\n";
		}
		while(confirmedPass!=clearPassword);

		do
		{
			std::cout << "Make Account a Game Master? (Y/N)\n";
			ReadAndWrite::getInputAsString(choice);
		}
		while (choice!="Y"&&choice!="y"&&choice!="N"&&choice!="n");
		bool bIsGameMaster;
		if(choice=="y"||choice=="Y")
			bIsGameMaster=true;
		else bIsGameMaster=false;

		std::string salt = picosha2::hash256_hex_string(std::to_string(getNanosSinceEpoch()));
		std::string hashedPass = picosha2::hash256_hex_string(clearPassword+salt);

		// obfuscate the salt and the hashed password
		std::string databaseEntry="";
		int strIndex=0;
		for(int i=0;i<128;i++)
		{
			if(i%2==0)
				databaseEntry+=hashedPass.at(strIndex);
			else
			{
				databaseEntry+=salt.at(strIndex);
				strIndex++;
			}
		}

		database->push_back(databaseEntry);
		database->push_back("}");
		std::cout << "*** Account Created Successfully ***\n";
		std::cout << "*** Logging in ***\n";
		instance = new GameInstance(accIndex,database,bIsGameMaster);
	}
	instance->saveChanges();
	CookieUI* gameUI = new CookieUI(instance);

	int choiceNum=-1;
	std::thread tCookiePerSecond(applyCookiePerSecond,instance);

	FindUserUI* findUI = new FindUserUI(instance);
	while(choiceNum!=CookieUI::Options::Logout)
	{
		CLEAR_SCREEN
		choice="";
		gameUI->display();
		choiceNum=-1;
		if(!gameUI->userInteraction(choice))
			choiceNum = gameUI->mapInputToEnum(choice);
		switch(choiceNum)
		{
			case CookieUI::Options::Click:
			{
				instance->addCookie(instance->getCookieClickRate());
				break;
			}
			case CookieUI::Options::PurchaseGrandmother:
			{
				if(!instance->subtractCookie(8))
					std::cout << "!! Not Enough Funds !!\n";
				else
					instance->addToRate(2.f);
				break;
			}
			case CookieUI::Options::PurchaseGreatGrandmother:
			{
				if(!instance->subtractCookie(200))
					std::cout << "!! Not Enough Funds !!\n";
				else
					instance->addToRate(4.f);
				break;
			}
			case CookieUI::Options::PurchaseSuperOven:
			{
				if(!instance->subtractCookie(10000000))
					std::cout << "!! Not Enough Funds !!\n";
				else
					instance->addToRate(999.f);
				break;
			}
			case CookieUI::Options::FindFriend:
			{
				std::string findUserChoice="";
				do
				{
					findUI->display();
				}
				while(findUI->userInteraction(findUserChoice));
				if(findUserChoice!="")
					instance->follow(findUserChoice,findUI->getChoices());
				break;
			}
		}
	}
	instance->bDestroyed=true;
	tCookiePerSecond.join();
	return 0;
}
