#pragma once

#define NOMINMAX
#include <algorithm>
#include <DirectXMath.h>
#include <vector>
#include "Collider.h"
#include "Mesh.h"

class BoxCollider :
	public Collider
{
public:
	BoxCollider(const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&, Transform* t);
	BoxCollider(const Mesh&,  Transform*);
	~BoxCollider();

	void GenerateAABB();
	DirectX::XMFLOAT3 GetMin() const;
	DirectX::XMFLOAT3 GetMax() const;
	float GreatestPointAlongAxis(const DirectX::XMVECTOR&) const;


private:
	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 dimensions;
	DirectX::XMFLOAT3 rotatedDimensions;
	DirectX::XMFLOAT3 verts [8];

};