
#include "VertexShader.h"

VertexShader::VertexShader(Graphics &gfx, const std::wstring &filePath)
{
	D3DReadFileToBlob(filePath.c_str(), &m_Blob);
	GetDevice(gfx)->CreateVertexShader(
		m_Blob->GetBufferPointer(), 
		m_Blob->GetBufferSize(), 
		nullptr, 
		&m_Handle);
}

void VertexShader::Bind(Graphics &gfx)
{
	GetContext(gfx)->VSSetShader(m_Handle.Get(), nullptr, 0u);
}