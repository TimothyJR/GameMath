#include "Camera.h"

Camera::Camera()
{
	position = DirectX::XMFLOAT3(0.0f, 0.0f, -6.0f);
	xRotation = 0.0f;
	yRotation = 0.0f;
	direction = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&position),
		DirectX::XMVector3Transform(
			DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			DirectX::XMMatrixRotationRollPitchYaw(xRotation, yRotation, 0.0f)
		),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	)));

	DirectX::XMStoreFloat4x4(&projection, DirectX::XMMatrixIdentity());
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return view;
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projection;
}

void Camera::Update(float deltaTime)
{
	DirectX::XMStoreFloat3(&direction, DirectX::XMVector3Rotate(
		DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
		DirectX::XMQuaternionRotationRollPitchYaw(xRotation, yRotation, 0.0f)));

	DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(
		DirectX::XMLoadFloat3(&position),
		DirectX::XMLoadFloat3(&direction),
		DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	)));

	if (GetAsyncKeyState('W') & 0x8000)
	{
		position.x += direction.x * deltaTime;
		position.y += direction.y * deltaTime;
		position.z += direction.z * deltaTime;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		DirectX::XMFLOAT3 right;
		DirectX::XMStoreFloat3(&right, DirectX::XMVector3Cross(
			DirectX::XMLoadFloat3(&direction),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
		
		position.x += right.x * deltaTime;
		position.y += right.y * deltaTime;
		position.z += right.z * deltaTime;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		position.x -= direction.x * deltaTime;
		position.y -= direction.y * deltaTime;
		position.z -= direction.z * deltaTime;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		DirectX::XMFLOAT3 right;
		DirectX::XMStoreFloat3(&right, DirectX::XMVector3Cross(
			DirectX::XMLoadFloat3(&direction),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
		
		position.x -= right.x * deltaTime;
		position.y -= right.y * deltaTime;
		position.z -= right.z * deltaTime;
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		position.y += deltaTime;
	}

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
	{
		position.y -= deltaTime;
	}
}

void Camera::UpdateRotation(float mouseX, float mouseY)
{
	xRotation += mouseY * 0.005f;
	yRotation += mouseX * 0.005f;

	if (xRotation < -1.4f)
	{
		xRotation = -1.4f;
	}
	else if (xRotation > 1.4f)
	{
		xRotation = 1.4f;
	}
}

DirectX::XMFLOAT4X4 Camera::CalculateProjection(float aspectRatio)
{
	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//   the window resizes (which is already happening in OnResize() below)
	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,				// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projection, XMMatrixTranspose(P)); // Transpose for HLSL!

	return DirectX::XMFLOAT4X4();
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return position;
}
