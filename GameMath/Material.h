#pragma once
#include <DirectXMath.h>
#include <memory>
#include "SimpleShader.h"
#include "Texture.h"
#include "Camera.h"
#include "DirectionalLight.h"

class Material
{
public:
	Material(SimpleVertexShader*, SimplePixelShader*, const std::shared_ptr<Texture>&);
	void Draw(const Camera&, const DirectionalLight&, const DirectX::XMFLOAT4X4);

	SimpleVertexShader* GetVertexShader() const;
	SimplePixelShader* GetPixelShader() const;
	std::shared_ptr<Texture> GetTexture() const;
	DirectX::XMFLOAT4 GetColor() const;

	void SetTexture(Texture&);
	void SetColor(DirectX::XMFLOAT4);
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	std::shared_ptr<Texture> texture;
	DirectX::XMFLOAT4 color;
};