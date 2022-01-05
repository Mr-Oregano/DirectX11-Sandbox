#pragma once

#include <LightWindows.h>
#include <string>
#include <optional>
#include <memory>

#include "AppException.h"
#include "Graphics.h"

struct WindowProps
{
	int width;
	int height;
	std::wstring title;
};

class WindowException : public MyAppException
{
public:
	WindowException(int line, const char *fileName, HRESULT hr);

	const char* what() const noexcept override;
	virtual const char *GetType() const noexcept override;
	inline HRESULT GetErrorCode() const noexcept { return hr; }
	inline std::string GetErrorString() const noexcept { return TranslateErrorCode(hr); }
private:
	HRESULT hr;

public:
	static std::string TranslateErrorCode(HRESULT hr);
};

class DX11Window
{
public:
	DX11Window(const WindowProps &props);
	~DX11Window();

	void ShowWindow();
	std::optional<int> PollEvents();

	inline Graphics& GetGfx() { return *m_Gfx; }

private:
	WindowProps m_Props;
	HINSTANCE m_HInstance;
	HWND m_Handle;

	std::unique_ptr<Graphics> m_Gfx;

private:
	static LRESULT CALLBACK MessageCallbackSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MessageCallbackStatic(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MessageCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	
	static bool s_RegisteredClass;

};

#define WIN_EXCEPT(hr) WindowException(__LINE__, __FILE__, hr)
#define WIN_EXCEPT_LAST() WindowException(__LINE__, __FILE__, GetLastError())