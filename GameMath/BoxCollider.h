#pragma once
#define NOMINMAX
#include <DirectXMath.h>
#include <algorithm>
#include <vector>
#include "Transform.h"
#include "Mesh.h"

class BoxCollider
{
public:
	BoxCollider(DirectX::XMFLOAT3&, DirectX::XMFLOAT3&, Transform& t);
	BoxCollider(Mesh&, Transform&);
	~BoxCollider();

	void GenerateAABB();
	bool CheckCollision(BoxCollider& box);

private:
	DirectX::XMFLOAT3 GetMin();
	DirectX::XMFLOAT3 GetMax();
	bool SeparateAxisTest(BoxCollider& box);
	float GreatestPointAlongAxis(DirectX::XMVECTOR&);

	DirectX::XMFLOAT3 center;
	DirectX::XMFLOAT3 dimensions;
	DirectX::XMFLOAT3 rotatedDimensions;
	DirectX::XMFLOAT3* verts;
	Transform* transform;
};