#include "pch.h"

//unused, but maybe someday in a future update?

int Difficulty;

void ReadConfig(const char* path) {

	const IniFile* config = new IniFile(std::string(path) + "\\config.ini");


	delete config;

}