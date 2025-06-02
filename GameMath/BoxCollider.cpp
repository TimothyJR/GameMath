#include "BoxCollider.h"

BoxCollider::BoxCollider(const DirectX::XMFLOAT3& c, const DirectX::XMFLOAT3& d, Transform* t) :
	Collider(t), center(c), dimensions(d)
{
	GenerateAABB();
}

BoxCollider::BoxCollider(const Mesh& mesh, Transform* t) :
	Collider(t),
	center(mesh.GetCentroid()),
	dimensions(
		{
		mesh.GetMax().x - mesh.GetMin().x,
		mesh.GetMax().y - mesh.GetMin().y,
		mesh.GetMax().z - mesh.GetMin().z
		}),
	rotatedDimensions(dimensions)
{
	GenerateAABB();
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::GenerateAABB()
{
	float halfWidth = dimensions.x / 2;
	float halfHeight = dimensions.y / 2;
	float halfDepth = dimensions.z / 2;


	verts[0] = DirectX::XMFLOAT3(-halfWidth, -halfHeight, -halfDepth);
	verts[1] = DirectX::XMFLOAT3(-halfWidth, -halfHeight, halfDepth);
	verts[2] = DirectX::XMFLOAT3(-halfWidth, halfHeight, -halfDepth);
	verts[3] = DirectX::XMFLOAT3(-halfWidth, halfHeight, halfDepth);
	verts[4] = DirectX::XMFLOAT3(halfWidth, halfHeight, halfDepth);
	verts[5] = DirectX::XMFLOAT3(halfWidth, halfHeight, -halfDepth);
	verts[6] = DirectX::XMFLOAT3(halfWidth, -halfHeight, halfDepth);
	verts[7] = DirectX::XMFLOAT3(halfWidth, -halfHeight, -halfDepth);

	for (int i = 0; i < 8; i++)
	{
		verts[i] = transform->GetRotation().RotateVector(verts[i]);
	}

	DirectX::XMFLOAT3 min = verts[0];
	DirectX::XMFLOAT3 max = verts[0];

	for (int i = 1; i < 8; i++)
	{
		min.x = std::min(min.x, verts[i].x);
		max.x = std::max(max.x, verts[i].x);
		min.y = std::min(min.y, verts[i].y);
		max.y = std::max(max.y, verts[i].y);
		min.z = std::min(min.z, verts[i].z);
		max.z = std::max(max.z, verts[i].z);
	}

	rotatedDimensions =
	{
		max.x - min.x,
		max.y - min.y,
		max.z - min.z
	};
}

DirectX::XMFLOAT3 BoxCollider::GetMin() const
{
	return DirectX::XMFLOAT3(
		transform->GetPosition().x - rotatedDimensions.x / 2,
		transform->GetPosition().y - rotatedDimensions.y / 2,
		transform->GetPosition().z - rotatedDimensions.z / 2);
}

DirectX::XMFLOAT3 BoxCollider::GetMax() const
{
	return DirectX::XMFLOAT3(
		transform->GetPosition().x + rotatedDimensions.x / 2,
		transform->GetPosition().y + rotatedDimensions.y / 2,
		transform->GetPosition().z + rotatedDimensions.z / 2);
}

float BoxCollider::GreatestPointAlongAxis(const DirectX::XMVECTOR& axis) const
{
	float greatest = 0.0f;
	DirectX::XMFLOAT3 point;
	DirectX::XMVECTOR pointV;

	for (int i = 0; i < 8; i++)
	{
		point = DirectX::XMFLOAT3(verts[i].x + transform->GetPosition().x,
			verts[i].y + transform->GetPosition().y,
			verts[i].z + transform->GetPosition().z);

		pointV = DirectX::XMLoadFloat3(&point);

		greatest = std::max(
			greatest,
			std::abs(
				DirectX::XMVectorGetByIndex(
					DirectX::XMVector3Dot(
						pointV,
						axis),
					0)));

	}

	return greatest;
}
