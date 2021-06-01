#include "Material.h"

Material::Material(SimpleVertexShader* v, SimplePixelShader* p, Texture* tex)
	: texture(tex)
{
	vertexShader = v;
	pixelShader = p;
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void Material::Draw(Camera& camera, DirectionalLight& light,DirectX::XMFLOAT4X4 world)
{
	vertexShader->SetMatrix4x4("view", camera.GetViewMatrix());
	vertexShader->SetMatrix4x4("projection", camera.GetProjectionMatrix());
	vertexShader->SetMatrix4x4("world", world);

	pixelShader->SetData("light", &light, sizeof(DirectionalLight));
	pixelShader->SetData("color", &color, sizeof(DirectX::XMFLOAT4));
	pixelShader->SetSamplerState("basicSampler", texture->GetSampler());
	pixelShader->SetShaderResourceView("diffuseTexture", texture->GetResourceView());

	vertexShader->SetShader(true);
	pixelShader->SetShader(true);
}

SimpleVertexShader* Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader* Material::GetPixelShader()
{
	return pixelShader;
}

Texture* Material::GetTexture()
{
	return texture;
}

DirectX::XMFLOAT4 Material::GetColor()
{
	return color;
}

void Material::SetTexture(Texture& tex)
{
	texture = &tex;
}

void Material::SetColor(DirectX::XMFLOAT4 c)
{
	color = c;
}
