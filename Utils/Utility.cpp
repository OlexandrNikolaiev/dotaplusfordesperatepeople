#include "Utility.h"
#include <iostream>
#include <windows.h>
std::string Utility::to_hex(uintptr_t value)
{
	std::ostringstream oss;
	oss << "0x" << std::hex << std::uppercase << value;
	return oss.str();
}

bool Utility::isValidTimeFormat(char* timeInGame) 
{
	LOG("Text for regex: " + std::string(timeInGame));
	std::regex pattern(R"(\d{1,3}:\d{2})");
	return std::regex_match(timeInGame, pattern);
}
