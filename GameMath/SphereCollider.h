#pragma once
#include <DirectXMath.h>
#include "Collider.h"
#include "Transform.h"
#include "Mesh.h"

class SphereCollider :
	public Collider
{
public:
	SphereCollider(DirectX::XMFLOAT3&, float, Transform* t);
	SphereCollider(const Mesh& mesh, Transform* t);

	DirectX::XMFLOAT3 GetCenter() const;
	DirectX::XMFLOAT3 GetWorldCenter() const;
	float GetRadius() const;

	void SetCenter(DirectX::XMFLOAT3&);
	void SetRadius(float);
private:
	DirectX::XMFLOAT3 center;
	float radius;
};