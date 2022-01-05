#pragma once

#include "Bindable.h"
#include "Graphics.h"

#include <vector>

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics &gfx, const std::vector<unsigned short> &indices);
	void Bind(Graphics &gfx) override;
	inline unsigned int GetCount() const { return m_Count; };

private:
	unsigned int m_Count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_Handle;
};