#pragma once

#include "Bindable.h"
#include "Graphics.h"

#include <string>

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics &gfx, const std::wstring& filePath);
	void Bind(Graphics &gfx) override;
	inline Microsoft::WRL::ComPtr<ID3DBlob>& GetBlob() { return m_Blob; }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_Handle;
	Microsoft::WRL::ComPtr<ID3DBlob> m_Blob;
};