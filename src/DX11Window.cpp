
#include "DX11Window.h"

#include <sstream>

#define CLASS_NAME L"DIRECTX FART BOX"

bool DX11Window::s_RegisteredClass = false;

DX11Window::DX11Window(const WindowProps &props)
	: m_Props(props), m_HInstance(GetModuleHandle(nullptr))
{
	if (!s_RegisteredClass)
	{
		WNDCLASSEX wc = {};
		wc.lpszClassName = CLASS_NAME;
		wc.hInstance = m_HInstance;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpfnWndProc = &DX11Window::MessageCallbackSetup;
		wc.hCursor = LoadCursor(m_HInstance, IDC_ARROW);
		wc.style = CS_OWNDC;

		if (!RegisterClassEx(&wc))
			throw WIN_EXCEPT_LAST();

		s_RegisteredClass = true;
	}

	RECT rect = { 0, 0, props.width, props.height };
	DWORD style = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	AdjustWindowRectEx(&rect, style, false, 0);

	m_Handle = CreateWindowEx(0, CLASS_NAME, props.title.c_str(),
		style, CW_USEDEFAULT, CW_USEDEFAULT, 
		rect.right - rect.left, rect.bottom - rect.top,
		nullptr, nullptr, m_HInstance, static_cast<void*>(this));

	if (!m_Handle)
		throw WIN_EXCEPT_LAST();

	m_Gfx = std::make_unique<Graphics>(m_Handle);
}

DX11Window::~DX11Window()
{
	DestroyWindow(m_Handle);
}

void DX11Window::ShowWindow()
{
	::ShowWindow(m_Handle, SW_NORMAL);
}

std::optional<int> DX11Window::PollEvents()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return { (int) msg.wParam };

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

LRESULT CALLBACK DX11Window::MessageCallbackSetup(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	if (Msg == WM_NCCREATE)
	{
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);

		DX11Window *self = static_cast<DX11Window*>(createStruct->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR) self);
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR) &DX11Window::MessageCallbackStatic);

		return self->MessageCallbackStatic(hWnd, Msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK DX11Window::MessageCallbackStatic(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	DX11Window *self = reinterpret_cast<DX11Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	return self->MessageCallback(hWnd, Msg, wParam, lParam);
}

LRESULT CALLBACK DX11Window::MessageCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
		PostQuitMessage(69);
		return 0;

	case WM_KEYUP:
		break;

	case WM_KEYDOWN:
		break;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

WindowException::WindowException(int line, const char *fileName, HRESULT hr)
	: MyAppException(line, fileName), hr(hr)
{}

const char *WindowException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char *WindowException::GetType() const noexcept
{
	return "DX11Window Exception";
}

std::string WindowException::TranslateErrorCode(HRESULT hr)
{
	char *pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&pMsgBuf), 0, nullptr);

	if (nMsgLen == 0)
		return "Unidentified error code";
	
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

