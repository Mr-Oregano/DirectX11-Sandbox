
#include "InputLayout.h"

InputLayout::InputLayout(
	Graphics &gfx,
	const std::vector<D3D11_INPUT_ELEMENT_DESC> &layout,
	Microsoft::WRL::ComPtr<ID3DBlob> &pVSByteCode)
{
	GetDevice(gfx)->CreateInputLayout(
		layout.data(), 
		(UINT) layout.size(),
		pVSByteCode->GetBufferPointer(),
		pVSByteCode->GetBufferSize(),
		&m_Handle);
}

void InputLayout::Bind(Graphics &gfx)
{
	GetContext(gfx)->IASetInputLayout(m_Handle.Get());
}