#pragma once
#include <DirectXMath.h>
#include "Transform.h"
#include "Mesh.h"

class SphereCollider
{
public:
	SphereCollider(DirectX::XMFLOAT3&, float, const std::shared_ptr<Transform>& t);
	SphereCollider(const Mesh& mesh, const std::shared_ptr<Transform>& t);

	DirectX::XMFLOAT3 GetCenter() const;
	float GetRadius() const;

	void SetCenter(DirectX::XMFLOAT3&);
	void SetRadius(float);

	bool CheckCollision(const SphereCollider& collider) const;

private:
	DirectX::XMFLOAT3 center;
	float radius;
	std::shared_ptr<Transform> transform;
};