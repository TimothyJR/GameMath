#include "Texture.h"

Texture::Texture(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const wchar_t* path)
{
	DirectX::CreateWICTextureFromFile(&device, &deviceContext, path, nullptr, &resourceView);

	D3D11_SAMPLER_DESC builder;
	ZeroMemory(&builder, sizeof(builder));
	builder.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	builder.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	builder.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	builder.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	builder.MaxLOD = D3D11_FLOAT32_MAX;
	HR(device.CreateSamplerState(&builder, &sampler));
}

Texture::~Texture()
{
	delete resourceView;
	delete sampler;
}

ID3D11ShaderResourceView* Texture::GetResourceView() const
{
	return resourceView;
}

ID3D11SamplerState* Texture::GetSampler() const
{
	return sampler;
}
