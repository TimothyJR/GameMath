#pragma once
#include <DirectXMath.h>
#include "Quaternion.h"

class Transform
{
public:
	Transform();
	Transform(DirectX::XMFLOAT3& pos);
	Transform(DirectX::XMFLOAT3& pos, Quaternion& rot, DirectX::XMFLOAT3& s);
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT3 GetPosition();
	Quaternion GetRotation();
	DirectX::XMFLOAT3 GetScale();

	void SetPosition(DirectX::XMFLOAT3&);
	void SetRotation(Quaternion&);
	void SetScale(DirectX::XMFLOAT3&);

private:
	DirectX::XMFLOAT3 position;
	Quaternion rotation;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4X4 worldMatrix;
	bool isDirty = false;
};