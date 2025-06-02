#include "Material.h"

Material::Material(SimpleVertexShader* v, SimplePixelShader* p, Texture* tex) :
	vertexShader(v),
	pixelShader(p),
	texture(tex),
	color(1.0f, 1.0f, 1.0f, 1.0f)
{
}

void Material::Draw(const Camera& camera, const DirectionalLight& light, const DirectX::XMFLOAT4X4 world) const
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

SimpleVertexShader* Material::GetVertexShader() const
{
	return vertexShader;
}

SimplePixelShader* Material::GetPixelShader() const
{
	return pixelShader;
}

Texture* Material::GetTexture() const
{
	return texture.get();
}

DirectX::XMFLOAT4 Material::GetColor() const
{
	return color;
}

void Material::SetTexture(Texture& tex)
{
	texture.reset(&tex);
}

void Material::SetColor(DirectX::XMFLOAT4 c)
{
	color = c;
}
