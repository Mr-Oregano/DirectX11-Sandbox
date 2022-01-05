#pragma once

#include "Bindable.h"
#include "Graphics.h"

template <typename T>
class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics &gfx, const T &item)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.ByteWidth = sizeof(item);
		desc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = &item;

		GetDevice(gfx)->CreateBuffer(&desc, &srd, &m_Handle);
	}

	void Update(Graphics &gfx, const T &item)
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		GetContext(gfx)->Map(m_Handle.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
		memcpy(msr.pData, &item, sizeof(item));
		GetContext(gfx)->Unmap(m_Handle.Get(), 0u);
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Handle;
};

template <typename T>
class VertexConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::m_Handle;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics &gfx) override
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, m_Handle.GetAddressOf());
	}
};

template <typename T>
class PixelConstantBuffer : public ConstantBuffer<T>
{
	using ConstantBuffer<T>::m_Handle;
	using Bindable::GetContext;
public:
	using ConstantBuffer<T>::ConstantBuffer;
	void Bind(Graphics &gfx) override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, m_Handle.GetAddressOf());
	}
};