#pragma once

#include "Bindable.h"
#include "Graphics.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics &gfx);
	void Bind(Graphics &gfx);

private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Handle;
};