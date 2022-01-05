
#include "Texture.h"

#include "stb_image.h"

Texture::Texture(Graphics &gfx, const std::string &filePath)
{
	int width;
	int height;
	int channels;
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);

	if (!data || channels != 4)
	{
		// OOPS! Something went wrong!
		__debugbreak();
		return;
	}

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = (UINT) width;
	desc.Height = (UINT) height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA srd = {};
	srd.pSysMem = data;
	srd.SysMemPitch = (UINT) (width * channels);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	GetDevice(gfx)->CreateTexture2D(&desc, &srd, &pTexture);
	stbi_image_free(data);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	GetDevice(gfx)->CreateShaderResourceView(pTexture.Get(), &srvDesc, &m_Handle);
}

void Texture::Bind(Graphics &gfx)
{
	GetContext(gfx)->PSSetShaderResources(0, 1, m_Handle.GetAddressOf());
}