#include <Windows.h>

#include "../UI.h"

int wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	UI::Run();

	return 0;
}