#pragma once
#include <WICTextureLoader.h>
#include "DirectXGameCore.h"
#include "d3d11.h"

class Texture
{
public:
	Texture(ID3D11Device&, ID3D11DeviceContext&, const wchar_t* path);
	~Texture();
	ID3D11ShaderResourceView* GetResourceView() const;
	ID3D11SamplerState* GetSampler() const;

private:
	ID3D11ShaderResourceView* resourceView;
	ID3D11SamplerState* sampler;
};