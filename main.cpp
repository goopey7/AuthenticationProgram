//
// Created by sam on 29/09/2020.
//

#include <future>
#include "MainMenu.h"
#include "sha256.h"
#include "GameInstance.h"
#include "CookieUI.h"
#include "FindUserUI.h"

void pressEnterToContinue();
bool isPasswordValid(std::string pass);

void play(std::string &choice, GameInstance* instance, CookieUI*&gameUI, FindUserUI*&findUI);

// this function runs on a separate thread adding the player's cookies per second every second.
void applyCookiePerSecond(GameInstance* instance)
{
	while(!instance->destroyed()) // if we are logged in
	{
		if(instance->getCookieRate()>0) // if we have a cookie rate
			instance->addCookie(instance->getCookieRate()); // add cookies
		WAIT_ONE_SECOND // wait one second
	}
}

//inline functions don't clog up the stack, instead the compiler just inserts the definition of the function when called
// returns an unsigned long long (max value: 18,446,744,073,709,551,615)
inline uint64_t getNanosSinceEpoch()
{
	// we use system clock because we want unix epoch, not time since program has been opened
	std::chrono::time_point<std::chrono::system_clock> ts = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(ts.time_since_epoch()).count();
}

// Yes I'm fully aware that I can put main above this function, and it'll work
// But look! I can pass functions into functions!
int login(std::vector<std::string>* database, GameInstance* &instance, int (*main)())
{
	bool bLoginFail=true;
	do
	{
		CLEAR_SCREEN // since it's a macro, we don't need a semicolon :-)
		std::cout << "Enter username: ";
		std::string username;
		ReadAndWrite::getInputAsString(username); // passed by reference
		std::cout << "Enter password: ";
		std::string password;
		ReadAndWrite::getInputAsString(password);
		size_t accIndex=-1; // the index of the line in the file (lineNumber - 1)
		for(int i=0;i<database->size();i++)
		{
			// if we find the username entered, we set our accIndex to the line in the database with the username
			if("ID:"+username==database->at(i))
			{
				accIndex=i;
				break;
			}
		}
		if(accIndex!=-1) //if we found a username
		{
			// the salted hash and the salt are stored 2 lines below the username in the file
			std::string hashLine=database->at(accIndex+2);

			// Extract the hash and the salt from the database entry
			std::string hash="";
			std::string salt="";
			// every other character is part of the hash
			// remaining characters are part of the salt
			for(int i=0;i<hashLine.length();i++)
			{
				if(i%2==0) //character is part of the hash
					hash+=hashLine.at(i);
				else //character is part of the salt
					salt+=hashLine.at(i);
			}
			// verify password
			if(hash==picosha2::hash256_hex_string(password+salt))
			{
				bLoginFail=false;
				std::cout << "*** Login Success ***\n";
				// we only need to set bIsGameMaster and Nickname when creating a new account
				// otherwise we read from the database.
				instance = new GameInstance(accIndex,database);
			}
		}
		if(bLoginFail)
		{
			std::cout << "ERROR: Invalid Login\n";
			pressEnterToContinue();
			return main();
		}
	}
	while(bLoginFail);
	return 91602;
}

void createAccount(std::vector<std::string>* database, GameInstance* &instance)
{
	//Create account
	std::string newAccountName;

	// Ask for account name and check if it's valid
	bool bBadUserName=false;
	do
	{
		CLEAR_SCREEN
		bBadUserName=false;
		std::cout << "Enter in a username for this account: ";
		ReadAndWrite::getInputAsString(newAccountName);
		for(std::string s : *database) //dereference the database pointer to get the actual database
		{
			// if account name is already in the database
			if(s=="ID:"+newAccountName)
			{
				bBadUserName = true;
				std::cout << "!! Account Name Already Exists !!\n";
			}
				// if account name contains a comma
			else if(newAccountName.find(',')!=std::string::npos)
			{
				bBadUserName=true;
				std::cout << "!! Commas Are Illegal !!\n";
				break;
			}
				// if account name contains a colon
			else if(newAccountName.find(':')!=std::string::npos)
			{
				bBadUserName=true;
				std::cout << "!! Colons are Illegal !!\n";
				break;
			}
			// note that spaces are allowed.
			// they don't break my program and I don't have a problem with users using spaces.
		}
		if(bBadUserName) // we want the user to acknowledge the error before looping back and clearing screen
			pressEnterToContinue();
	}
	while (bBadUserName);
	// after this point we have a valid account name.
	newAccountName = "ID:"+newAccountName;
	//we are going to add a new line to the database, so this is going to be the new index.
	size_t accIndex = database->size();
	database->push_back(newAccountName);
	database->push_back("{");

	// Ask for a password
	std::string clearPassword;
	std::string confirmedPass;
	do
	{
		do
		{
			std::cout << "Enter in a password for this account: ";
			ReadAndWrite::getInputAsString(clearPassword);
			if(!isPasswordValid(clearPassword)) //if password does not meet criteria
			{
				std::cout << "!! Invalid Password !!\n";
				std::cout << "Password should contain the following:\n";
				std::cout << "At least 8 characters\n";
				std::cout << "A Number\nA Special Character\nA Lowercase letter\nAn Uppercase Letter\n";
				pressEnterToContinue();
			}
		}
		while(!isPasswordValid(clearPassword)); // iterate until we have a good password
		std::cout << "Confirm password: ";
		ReadAndWrite::getInputAsString(confirmedPass);
		if(confirmedPass!=clearPassword)
		{
			std::cout << "!! Passwords do not match !!\n";
			pressEnterToContinue();
		}
	}
	while(confirmedPass!=clearPassword); // iterate until user confirms a valid password

	std::string choice;
	// ask user whether or not they'd like to be a game master (administrator)
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

	// ask user for a name (it is ok to have duplicates of these in the database)
	std::cout << "Enter in a name: ";
	std::string nickname;
	ReadAndWrite::getInputAsString(nickname);

	// acquire a salt based on doing sha256 on the nanoseconds since epoch
	std::string salt = picosha2::hash256_hex_string(std::to_string(getNanosSinceEpoch()));
	// hash our our password with the hashed salt appended to it
	std::string hashedPass = picosha2::hash256_hex_string(clearPassword+salt);

	// obfuscate the salt and the hashed password in the database
	std::string hashDatabaseEntry="";
	int strIndex=0;
	// make every even character the hash, and every odd character the salt
	for(int i=0;i<128;i++) // sha256 hashes are 64 characters long
	{
		if(i%2==0)
			hashDatabaseEntry+=hashedPass.at(strIndex);
		else
		{
			hashDatabaseEntry+=salt.at(strIndex);
			strIndex++;//only increment on every odd character, since we are working on the same index for each hash
		}
	}
	// add to end of the database
	database->push_back(hashDatabaseEntry);
	// complete database entry with curley
	database->push_back("}");
	std::cout << "*** Account Created Successfully ***\n";
	std::cout << "*** Logging in ***\n";
	instance = new GameInstance(accIndex,database,bIsGameMaster,nickname);
}

int main()
{
	MainMenu* mainMenu=new MainMenu;
	std::string choice;
	do
	{
		choice="";
		CLEAR_SCREEN
		mainMenu->display();
	}
	while (mainMenu->userInteraction(choice)); // while user input is invalid
	// read the database into memory
	std::vector<std::string>* database = ReadAndWrite::readFile("database.txt");
	GameInstance* instance=nullptr;
	if(choice=="0")return 0; // exit program
	if(choice=="1") //we use == for strings in C++ because of operator overloading :-)
	{
		int retrnVal=login(database,instance,main);
		// this is so that we can exit after failing to login
		if(retrnVal!=91602) // returns this number if we made it to the end of the method
			return retrnVal;
	}
	else if(choice=="2")
		createAccount(database,instance);

	// at this point we are logged into an account
	// save current database to the file
	instance->saveChanges();

	// fire up our thread which adds cookies per second every second
	std::thread tCookiePerSecond(applyCookiePerSecond,instance);
	CookieUI* gameUI;
	FindUserUI* findUI;
	instance->play(choice, gameUI, findUI,pressEnterToContinue);

	// if we are logging out
	instance->bDestroyed=true; // this will alert the thread to stop
	tCookiePerSecond.join(); // wait for thread to finish

	// garbage collection
	delete instance;
	delete gameUI;
	delete findUI;
	delete mainMenu;

	// return to main menu
	return main();
}

void pressEnterToContinue()
{
	std::cout << "Press enter to continue";
	std::string irrelevent; // we just need to pause the execution until user hits enter key.
	ReadAndWrite::getInputAsString(irrelevent);
}

bool isPasswordValid(std::string pass)
{
	if(pass.length()>=8) // must be at least 8 characters long
	{
		char specials[] = {'!','@','#','$','%','^','&','*','~','`',';','\''};
		char numbers[] = {'0','1','2','3','4','5','6','7','8','9'};
		bool bHasSpecialChars=false;
		bool bHasUpperCaseChars=false;
		bool bHasNumberChars=false;
		bool bHasLowercaseChars=false;
		for(char c : pass)
		{
			for (char spec : specials)
			{
				if (c == spec)
				{
					bHasSpecialChars = true;
					break;
				}
			}
			for(char num : numbers)
			{
				if(c==num)
				{
					bHasNumberChars=true;
					break;
				}
			}
			if(std::isupper(c))bHasUpperCaseChars=true;
			if(std::islower(c))bHasLowercaseChars=true;
		}
		// if we follow the criteria
		if(bHasLowercaseChars&&bHasUpperCaseChars&&bHasNumberChars&&bHasSpecialChars)
			return true;
	}
	return false;
}