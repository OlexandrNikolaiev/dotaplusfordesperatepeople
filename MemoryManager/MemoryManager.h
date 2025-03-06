#pragma once

#include <Windows.h>
#include <vector>

#include "../ProcessManager/ProcessManager.h"

class MemoryManager
{
public:
	MemoryManager(char* timeInGame);
	
	bool ReadMemory();
	bool ReadPointerChain(int moduleID, uintptr_t moduleOffset, const std::vector<uintptr_t>& offsets);

private:
	ProcessManager _processManager;
	uintptr_t _finalAddress;
	HANDLE _hProcess;

    char* _timeInGame;
};

