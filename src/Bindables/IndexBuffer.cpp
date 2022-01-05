
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics &gfx, const std::vector<unsigned short> &indices)
	: m_Count((unsigned int) indices.size())
{
	// Index Buffer Descriptor
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = (UINT) (indices.size() * sizeof(unsigned short));
	desc.StructureByteStride = (UINT) sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = indices.data();

	GetDevice(gfx)->CreateBuffer(&desc, &srd, &m_Handle);
}

void IndexBuffer::Bind(Graphics &gfx)
{
	GetContext(gfx)->IASetIndexBuffer(m_Handle.Get(), DXGI_FORMAT_R16_UINT, 0);
}