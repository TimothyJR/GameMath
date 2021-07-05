#pragma once
#include <DirectXMath.h>
#include "Quaternion.h"

class Transform
{
public:
	Transform();
	Transform(const DirectX::XMFLOAT3& pos);
	Transform(const DirectX::XMFLOAT3& pos, const Quaternion& rot, const DirectX::XMFLOAT3& s);
	Transform(const Transform&);
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetWorldMatrix() const;
	DirectX::XMFLOAT3 GetPosition() const;
	Quaternion GetRotation() const;
	DirectX::XMFLOAT3 GetScale() const;

	void SetPosition(const DirectX::XMFLOAT3&);
	void SetRotation(const Quaternion&);
	void SetScale(const DirectX::XMFLOAT3&);

private:
	void CalculateWorldMatrix(DirectX::XMFLOAT4X4& storage) const;

	DirectX::XMFLOAT3 position;
	Quaternion rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4X4 worldMatrix;
	bool isDirty = false;
};