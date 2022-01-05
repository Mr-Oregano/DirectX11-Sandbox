
#include "PixelShader.h"

PixelShader::PixelShader(Graphics &gfx, const std::wstring &filePath)
{
	using namespace Microsoft::WRL;
	ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(filePath.c_str(), &pBlob);

	GetDevice(gfx)->CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&m_Handle);
}

void PixelShader::Bind(Graphics &gfx)
{
	GetContext(gfx)->PSSetShader(m_Handle.Get(), nullptr, 0u);
}