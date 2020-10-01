//
// Created by sam on 30/09/2020.
//

#ifndef AUTHENTICATIONPROGRAM_READANDWRITE_H
#define AUTHENTICATIONPROGRAM_READANDWRITE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

class ReadAndWrite
{
public:
	static void getInputAsString(std::string& input);
	static std::vector<std::string>* readFile(std::string inFileName);
	static void writeFile(std::vector<std::string>* outFileContents,std::string outFileName);
};


#endif //AUTHENTICATIONPROGRAM_READANDWRITE_H
