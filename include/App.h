#pragma once

#include "DX11Window.h"
#include "ChiliTimer.h"

class App
{
public:
	App();
	int Run();

private:
	void Update();

private:
	DX11Window m_Window;
	ChiliTimer m_Timer;
};