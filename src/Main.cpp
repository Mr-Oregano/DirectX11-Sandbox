
#include "App.h"

int WINAPI wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR pCmdLine,
	_In_ int nCmdShow) 
{
	App app;
	return app.Run();
}