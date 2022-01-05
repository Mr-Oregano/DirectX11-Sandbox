
#include "Sampler.h"

Sampler::Sampler(Graphics &gfx)
{
	D3D11_SAMPLER_DESC desc = {};
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	GetDevice(gfx)->CreateSamplerState(&desc, &m_Handle);
}

void Sampler::Bind(Graphics &gfx)
{
	GetContext(gfx)->PSSetSamplers(0, 1, m_Handle.GetAddressOf());
}