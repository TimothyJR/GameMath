#pragma once
#define NOMINMAX
#include <algorithm>
#include <DirectXMath.h>
#include <vector>
#include "Mesh.h"
#include "Transform.h"

class BoxCollider
{
public:
	BoxCollider(const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&, const std::shared_ptr<Transform>& t);
	BoxCollider(const Mesh&, const std::shared_ptr<Transform>&);
	~BoxCollider();

	void GenerateAABB();
	bool CheckCollision(const BoxCollider& box) const;

private:
	DirectX::XMFLOAT3 GetMin() const;
	DirectX::XMFLOAT3 GetMax() const;
	bool SeparateAxisTest(const BoxCollider& box) const;
	float GreatestPointAlongAxis(const DirectX::XMVECTOR&) const;

	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 dimensions;
	DirectX::XMFLOAT3 rotatedDimensions;
	DirectX::XMFLOAT3 verts [8];
	std::shared_ptr<Transform> transform;
};