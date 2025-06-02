#include "Transform.h"

Transform::Transform() :
	position(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
	rotation(Quaternion()),
	scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
	isDirty(false)
{
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
}

Transform::Transform(const DirectX::XMFLOAT3& pos) :
	position(pos),
	rotation(Quaternion()),
	scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
	isDirty(true)
{
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
}

Transform::Transform(const DirectX::XMFLOAT3& pos, const Quaternion& rot, const DirectX::XMFLOAT3& s) :
	position(pos), rotation(rot), scale(s), isDirty(true)
{
	DirectX::XMStoreFloat4x4(&worldMatrix, DirectX::XMMatrixIdentity());
}

Transform::Transform(const Transform& t) :
	position(t.GetPosition()),
	rotation(t.GetRotation()),
	scale(t.GetScale()),
	worldMatrix(t.GetWorldMatrix()),
	isDirty(false)
{
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	if (isDirty)
	{
		CalculateWorldMatrix(worldMatrix);
		isDirty = false;
	}

	return worldMatrix;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix() const
{
	if (isDirty)
	{
		DirectX::XMFLOAT4X4 newWorld;
		CalculateWorldMatrix(newWorld);

		return newWorld;
	}

	return worldMatrix;
}

DirectX::XMFLOAT3 Transform::GetPosition() const
{
	return position;
}

Quaternion Transform::GetRotation() const
{
	return rotation;
}

DirectX::XMFLOAT3 Transform::GetScale() const
{
	return scale;
}

void Transform::SetPosition(const DirectX::XMFLOAT3& p)
{
	position = p;
	isDirty = true;
}

void Transform::SetRotation(const Quaternion& r)
{
	rotation = r;
	isDirty = true;
}

void Transform::SetScale(const DirectX::XMFLOAT3& s)
{
	scale = s;
	isDirty = true;
}

void Transform::CalculateWorldMatrix(DirectX::XMFLOAT4X4& storage) const
{
	// Multiply order -> Scale -> Rotation -> Translation
	DirectX::XMMATRIX t = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&position));

	DirectX::XMFLOAT4 rotationFloat4 = DirectX::XMFLOAT4(rotation.x, rotation.y, rotation.z, rotation.w);
	DirectX::XMMATRIX r = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&rotationFloat4));
	DirectX::XMMATRIX s = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&scale));
	DirectX::XMMATRIX w = DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(s, r), t);
	DirectX::XMStoreFloat4x4(&storage, DirectX::XMMatrixTranspose(w));
}
