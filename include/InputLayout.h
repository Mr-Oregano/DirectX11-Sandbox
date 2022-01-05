#pragma once

#include "Bindable.h"
#include "Graphics.h"

#include <vector>

class InputLayout : public Bindable
{
public:
	InputLayout(
		Graphics &gfx, 
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, 
		Microsoft::WRL::ComPtr<ID3DBlob>& pVSByteCode);

	void Bind(Graphics &gfx) override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_Handle;
};