#pragma once

#include "LightWindows.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <array>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>

class Graphics
{
	friend class Bindable;

public:
	Graphics(HWND hWnd);

	void Present();
	void ClearBuffer(float r, float g, float b);
	void DrawIndexed(unsigned int count);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_TargetBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;
};