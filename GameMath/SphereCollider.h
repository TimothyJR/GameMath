#pragma once
#include <DirectXMath.h>
#include "Transform.h"
#include "Mesh.h"

class SphereCollider
{
public:
	SphereCollider(DirectX::XMFLOAT3&, float, Transform&);
	SphereCollider(Mesh& mesh, Transform&);

	DirectX::XMFLOAT3 GetCenter();
	float GetRadius();

	void SetCenter(DirectX::XMFLOAT3&);
	void SetRadius(float);

	bool CheckCollision(SphereCollider& collider);

private:
	DirectX::XMFLOAT3 center;
	float radius;
	Transform* transform;
};