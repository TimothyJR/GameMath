#include "BoxCollider.h"

BoxCollider::BoxCollider(const DirectX::XMFLOAT3& c, const DirectX::XMFLOAT3& d, const std::shared_ptr<Transform>& t) :
	center(c), dimensions(d), transform(t)
{
	GenerateAABB();
}

BoxCollider::BoxCollider(const Mesh& mesh, const std::shared_ptr<Transform>& t) :
	center(mesh.GetCentroid()),
	dimensions(
		{
		mesh.GetMax().x - mesh.GetMin().x,
		mesh.GetMax().y - mesh.GetMin().y,
		mesh.GetMax().z - mesh.GetMin().z
		}),
	rotatedDimensions(dimensions),
	transform(t)
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

bool BoxCollider::CheckCollision(const BoxCollider& box) const
{
	DirectX::XMFLOAT3 minThis = GetMin();
	DirectX::XMFLOAT3 maxThis = GetMax();
	DirectX::XMFLOAT3 minOther = box.GetMin();
	DirectX::XMFLOAT3 maxOther = box.GetMax();


	if (maxThis.x < minOther.x || minThis.x > maxOther.x ||
		maxThis.y < minOther.y || minThis.y > maxOther.y ||
		maxThis.z < minOther.z || minThis.z > maxOther.z)
	{
		return false;
	}

	return SeparateAxisTest(box);
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

bool BoxCollider::SeparateAxisTest(const BoxCollider& box) const
{
	std::vector<DirectX::XMVECTOR> testAxis;

	testAxis.push_back(DirectX::XMLoadFloat3(&transform->GetRotation().RotateVector(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f))));
	testAxis.push_back(DirectX::XMLoadFloat3(&transform->GetRotation().RotateVector(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f))));
	testAxis.push_back(DirectX::XMLoadFloat3(&transform->GetRotation().RotateVector(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f))));
	testAxis.push_back(DirectX::XMLoadFloat3(&box.transform->GetRotation().RotateVector(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f))));
	testAxis.push_back(DirectX::XMLoadFloat3(&box.transform->GetRotation().RotateVector(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f))));
	testAxis.push_back(DirectX::XMLoadFloat3(&box.transform->GetRotation().RotateVector(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f))));

	for (int i = 0; i < 3; i++)
	{
		for (int j = 3; j < 6; j++)
		{
			if (std::abs(DirectX::XMVectorGetByIndex(DirectX::XMVector3Dot(testAxis[i], testAxis[j]), 0)) < 0.995f)
			{
				testAxis.push_back(DirectX::XMVector3Normalize(DirectX::XMVector3Cross(testAxis[i], testAxis[j])));
			}
		}
	}

	DirectX::XMFLOAT3 posDifference = DirectX::XMFLOAT3(
		transform->GetPosition().x - box.transform->GetPosition().x,
		transform->GetPosition().y - box.transform->GetPosition().y,
		transform->GetPosition().z - box.transform->GetPosition().z);

	DirectX::XMVECTOR positionDifference = DirectX::XMLoadFloat3(&posDifference);

	bool collided = false;

	for (unsigned i = 0; i < testAxis.size(); i++)
	{
		float distance = std::abs(
			DirectX::XMVectorGetByIndex(
				DirectX::XMVector3Dot(
					positionDifference,
					testAxis[i]),
				0));

		float leftSide = GreatestPointAlongAxis(testAxis[i]);
		float rightSide = box.GreatestPointAlongAxis(testAxis[i]);

		if (distance > leftSide + rightSide)
		{
			return false;
		}
	}

	return true;
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
