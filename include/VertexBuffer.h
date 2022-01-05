#pragma once

#include "Bindable.h"
#include "Graphics.h"

#include <vector>

class VertexBuffer : public	Bindable
{
public:
	template<typename V>
	VertexBuffer(Graphics &gfx, const std::vector<V> &vertices)
		: m_Stride(sizeof(V))
	{
		// Vertex Buffer Descriptor
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.ByteWidth = (UINT) (vertices.size() * sizeof(V));
		desc.StructureByteStride = sizeof(V);

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = vertices.data();

		GetDevice(gfx)->CreateBuffer(&desc, &srd, &m_Handle);
	}
	void Bind(Graphics &gfx) override;

private:
	unsigned int m_Stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Handle;
};