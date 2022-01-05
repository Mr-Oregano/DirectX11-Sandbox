#pragma once

#include "Graphics.h"

class Bindable
{
public:
	virtual ~Bindable() = default;
	virtual void Bind(Graphics &gfx) = 0;

protected:
	inline Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice(Graphics& gfx) const { return gfx.m_Device; }
	inline Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetContext(Graphics& gfx) const { return gfx.m_Context; }
};