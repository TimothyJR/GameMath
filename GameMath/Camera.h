#pragma once

#include <DirectXMath.h>
#include "Windows.h"

class Camera
{
public:
	Camera();
	DirectX::XMFLOAT4X4 GetViewMatrix() const;
	DirectX::XMFLOAT4X4 GetProjectionMatrix() const;
	void Update(const float deltaTime);
	void UpdateRotation(const float mouseX, const float mouseY);
	DirectX::XMFLOAT4X4 CalculateProjection(float aspectRatio);
	DirectX::XMFLOAT3 GetPosition() const;
private:
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	float xRotation;
	float yRotation;
};