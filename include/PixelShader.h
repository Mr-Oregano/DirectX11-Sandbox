#pragma once

#include "Bindable.h"
#include "Graphics.h"

#include <string>

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics &gfx, const std::wstring &filePath);
	void Bind(Graphics &gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_Handle;
};