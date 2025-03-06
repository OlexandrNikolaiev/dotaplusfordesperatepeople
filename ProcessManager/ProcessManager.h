#pragma once

#include <windows.h>
#include <TlHelp32.h>
#include <string>
#include <unordered_map>

#include "../Logger/Logger.h"

class ProcessManager {
public:
    ProcessManager();
    ~ProcessManager();

    HANDLE GetProcessHandle() const;
    uintptr_t getModuleMapAt(int moduleID);
    std::unordered_map<int, std::wstring> getModuleMap();
    bool findProcess(HANDLE& pHandle);
    bool findWindow();

private:
    HWND _hWnd;
    DWORD _pID;
    HANDLE _pHandle;

    std::unordered_map<int, uintptr_t> _cachedModuleAddresses;
    const std::unordered_map<int, std::wstring> _moduleMap = {
        { 136, L"panorama.dll" },
        { 128, L"client.dll" },
        { 127, L"server.dll" },
        { 124, L"particles.dll" },
        { 111, L"worldrenderer.dll" },
        { 103, L"materialsystem2.dll" },
        { 84,  L"resourcesystem.dll" },
        { 76,  L"rendersystemdx11.dll" },
        { 35,  L"engine2.dll" },
        { 33,  L"tier0.dll" }
    };

    uintptr_t GetModuleBaseAddress(const wchar_t* modName);
};

