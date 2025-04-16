#pragma once
#include <vector>
#include <string>
#include <Windows.h>

namespace Utils {
	std::vector<ProcEntity> GetProcessList();
	std::string StrToLower(std::string str);
}