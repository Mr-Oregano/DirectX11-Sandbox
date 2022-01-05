#pragma once

#include "Bindable.h"
#include "Graphics.h"

#include <string>

class Texture : public Bindable
{
public:
	Texture(Graphics &gfx, const std::string &filePath);
	void Bind(Graphics &gfx);

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Handle;
};