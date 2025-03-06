#include "MemoryManager.h"

MemoryManager::MemoryManager(char* timeInGame) : _processManager(), _finalAddress(0), _timeInGame(timeInGame) {

    _processManager.findProcess(_hProcess);
}

bool MemoryManager::ReadPointerChain(int moduleID, uintptr_t moduleOffset, const std::vector<uintptr_t>& offsets)
{
    if (!_processManager.findWindow()) {
        LOG("Couldn't get _hProcess, trying again after 1 second...");
        std::cout << "Couldn't get _hProcess, trying again after 1 second..." << std::endl;
        Sleep(1000);
        _processManager.findProcess(_hProcess);

        return false;
    }

    uintptr_t baseAddress = _processManager.getModuleMapAt(moduleID);
    std::unordered_map<int, std::wstring> moduleMap = _processManager.getModuleMap();
    std::string moduleName = std::string(moduleMap[moduleID].begin(), moduleMap[moduleID].end());
    _finalAddress = baseAddress + moduleOffset;

    uintptr_t addr = _finalAddress;
    for (uintptr_t offset : offsets) {
        if (!ReadProcessMemory(_hProcess, (LPCVOID)addr, &addr, sizeof(addr), nullptr)) {
            LOG(moduleName + ":    " + Utility::to_hex(moduleOffset) + ", " + Utility::to_hex(offsets.at(0)) + ", " + Utility::to_hex(offsets.at(1))
                + ", " + Utility::to_hex(offsets.at(2)) + ", " + Utility::to_hex(offsets.at(3)) + ", " + Utility::to_hex(offsets.at(4)) + ", "
                + Utility::to_hex(offsets.at(5)) + ", " + Utility::to_hex(offsets.at(6)));
            LOG("Failed to read memory at address: " + Utility::to_hex(addr) + " for module: " + moduleName);
            return false;
        }
        addr += offset;
    }
    LOG("The chain has been resolved");
    _finalAddress = addr;
    return true;
}

bool MemoryManager::ReadMemory()
{
    if (!_processManager.findWindow()) {
        return false;
    }
    if (!ReadProcessMemory(_hProcess, (LPCVOID)_finalAddress, _timeInGame, sizeof(_timeInGame), nullptr)) {
        LOG("Couldn't read the memory");
        return false;
    }
    return true;
}
