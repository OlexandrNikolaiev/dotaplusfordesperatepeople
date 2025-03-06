#include "ProcessManager.h"

ProcessManager::ProcessManager() 
{
    //
}

bool ProcessManager::findProcess(HANDLE& pHandle)
{
    if (!findWindow())
    {
        LOG("Window not found!");
        pHandle = nullptr;
        return false;
    }

    GetWindowThreadProcessId(_hWnd, &_pID);
    pHandle = OpenProcess(PROCESS_VM_READ, FALSE, _pID);
    if (!pHandle) {
        LOG("Failed to open process!");
        return false;
    }
    return true;
}

bool ProcessManager::findWindow()
{
    _hWnd = FindWindow(nullptr, L"Dota 2");
    if (!_hWnd) {
        return false;
    }
    return true;
}

ProcessManager::~ProcessManager()
{
    if (_pHandle) {
        CloseHandle(_pHandle);
    }
}

HANDLE ProcessManager::GetProcessHandle() const
{
    return _pHandle;
}

uintptr_t ProcessManager::GetModuleBaseAddress(const wchar_t* modName) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _pID);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                if (!_wcsicmp(modEntry.szModule, modName)) {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

uintptr_t ProcessManager::getModuleMapAt(int moduleID)
{
    if (_cachedModuleAddresses.count(moduleID)) {
        return _cachedModuleAddresses[moduleID];
    }
    else {
        auto it = _moduleMap.find(moduleID);
        if (it != _moduleMap.end()) {
            uintptr_t addr = GetModuleBaseAddress(it->second.c_str());
            _cachedModuleAddresses[moduleID] = addr;
            return addr;
        }
        LOG("Couldn't find the module " + std::to_string(moduleID));
    }
}

std::unordered_map<int, std::wstring> ProcessManager::getModuleMap()
{
    return _moduleMap;
}




