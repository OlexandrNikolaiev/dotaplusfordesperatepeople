#pragma once

#include <sstream>  
#include <regex>

#include "../Logger/Logger.h"

class Utility
{
public:
	static std::string to_hex(uintptr_t value);
	static bool isValidTimeFormat(char* timeInGame);
};

