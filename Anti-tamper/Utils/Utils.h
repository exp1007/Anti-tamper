#pragma once
#include <vector>
#include <string>
#include <Windows.h>

namespace Utils {
	struct ProcessEntity {
		std::string Name;
		DWORD PID;
	};

	std::vector<Utils::ProcessEntity> GetProcessList();
	std::string StrToLower(std::string str);
}