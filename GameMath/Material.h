#pragma once
#include <DirectXMath.h>
#include "SimpleShader.h"
#include "Texture.h"
#include "Camera.h"
#include "DirectionalLight.h"

class Material
{
public:
	Material(SimpleVertexShader*, SimplePixelShader*, Texture*);
	void Draw(Camera&, DirectionalLight&, DirectX::XMFLOAT4X4);

	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
	Texture* GetTexture();
	DirectX::XMFLOAT4 GetColor();

	void SetTexture(Texture&);
	void SetColor(DirectX::XMFLOAT4);
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	Texture* texture;
	DirectX::XMFLOAT4 color;
};