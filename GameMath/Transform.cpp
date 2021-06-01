#include "Transform.h"

Transform::Transform()
{
	position = { 0.0f, 0.0f, 0.0f };
	rotation = Quaternion();
	scale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
}

Transform::Transform(DirectX::XMFLOAT3& pos) :
	position(pos)
{
	rotation = Quaternion();
	scale = { 1.0f, 1.0f, 1.0f };
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	isDirty = true;
}

Transform::Transform(DirectX::XMFLOAT3& pos, Quaternion& rot, DirectX::XMFLOAT3& s) :
	position(pos), rotation(rot), scale(s)
{
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
	isDirty = true;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	if (isDirty)
	{
		// Multiply order -> Scale -> Rotation -> Translation
		DirectX::XMMATRIX t = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));
		DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(rotation.x, rotation.y, rotation.z, rotation.w)));
		DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale));
		DirectX::XMMATRIX w = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(s, r), t);
		DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixTranspose(w));

		isDirty = false;
	}

	return worldMatrix;
}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

Quaternion Transform::GetRotation()
{
	return rotation;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

void Transform::SetPosition(DirectX::XMFLOAT3& p)
{
	position = p;
	isDirty = true;
}

void Transform::SetRotation(Quaternion& r)
{
	rotation = r;
	isDirty = true;
}

void Transform::SetScale(DirectX::XMFLOAT3& s)
{
	scale = s;
	isDirty = true;
}
