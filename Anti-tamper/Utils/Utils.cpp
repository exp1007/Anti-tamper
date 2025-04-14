#include "Utils.h"

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <string>
#include <vector>
#include <unordered_set>

std::string GetProcName(DWORD processID)
{
    CHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
        PROCESS_VM_READ,
        FALSE, processID);

    if (NULL != hProcess)
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded))
        {
            GetModuleBaseName(hProcess, hMod, szProcessName,
                sizeof(szProcessName) / sizeof(TCHAR));
        }
    }

    return std::string(szProcessName);

    CloseHandle(hProcess);
}

namespace Utils {
    std::vector<Utils::ProcessEntity> GetProcessList() {
        DWORD aProcesses[1024], cbNeeded, cProcesses;
        unsigned int i;
        std::unordered_set<std::string> ProcWhiteList{
            "<unknown>",
            "svchost.exe",
            "winlogon.exe",
            "RuntimeBroker.exe",
            "conhost.exe",
            "dwm.exe"
        };

        if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
            return {};

        cProcesses = cbNeeded / sizeof(DWORD);

        std::vector<Utils::ProcessEntity> ProcessList;
        for (i = 0; i < cProcesses; i++) {
            if (aProcesses[i] == 0)
                continue;

            Utils::ProcessEntity TempProc;
            TempProc.Name = GetProcName(aProcesses[i]);
            if (ProcWhiteList.find(TempProc.Name) != ProcWhiteList.end())
                continue;

            TempProc.PID = aProcesses[i];

            ProcessList.push_back(TempProc);
        }

        return ProcessList;
    }

    std::string StrToLower(std::string str) {
        std::string FinalStr;
        for (uint32_t idx = 0; idx < str.size(); idx++) {
            FinalStr.push_back(std::tolower(str[idx]));
        }

        return FinalStr;
    }
}
