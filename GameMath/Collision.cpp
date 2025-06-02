#include "Collision.h"

bool Collision::CheckCollision(const BoxCollider& box1, const BoxCollider& box2)
{
	DirectX::XMFLOAT3 min1 = box1.GetMin();
	DirectX::XMFLOAT3 max1 = box1.GetMax();
	DirectX::XMFLOAT3 min2 = box2.GetMin();
	DirectX::XMFLOAT3 max2 = box2.GetMax();

	if (max1.x < min2.x || min1.x > max2.x ||
		max1.y < min2.y || min1.y > max2.y ||
		max1.z < min2.z || min1.z > max2.z)
	{
		return false;
	}

	return SeparateAxisTest(box1, box2);
}

bool Collision::CheckCollision(const SphereCollider& sphere1, const SphereCollider& sphere2)
{
	DirectX::XMFLOAT3 sphere1WorldCenter = sphere1.GetWorldCenter();
	DirectX::XMFLOAT3 sphere2WorldCenter = sphere2.GetWorldCenter();

	float distance = DirectX::XMVectorGetByIndex(DirectX::XMVector3Length(
		DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&sphere1WorldCenter),
			DirectX::XMLoadFloat3(&sphere2WorldCenter)
		)
	), 0);

	if (distance < sphere1.GetRadius() + sphere2.GetRadius())
	{
		return true;
	}

	return false;
}

bool Collision::CheckCollision(const BoxCollider& box, const SphereCollider& sphere)
{
	return CheckCollision(sphere, box);
}

bool Collision::CheckCollision(const SphereCollider& sphere, const BoxCollider& box)
{
	DirectX::XMFLOAT3 closestPoint = box.GetTransform()->GetPosition();
	DirectX::XMFLOAT3 direction = sphere.GetCenter();
	direction.x -= closestPoint.x;
	direction.y -= closestPoint.y;
	direction.z -= closestPoint.z;

	for (int i = 0; i < 3; i++)
	{

	}

	return false;
}

bool Collision::SeparateAxisTest(const BoxCollider& box1, const BoxCollider& box2)
{
	std::vector<DirectX::XMVECTOR> testAxis;
	DirectX::XMFLOAT3 box1X = box1.GetTransform()->GetRotation().RotateVector(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));
	DirectX::XMFLOAT3 box1Y = box1.GetTransform()->GetRotation().RotateVector(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
	DirectX::XMFLOAT3 box1Z = box1.GetTransform()->GetRotation().RotateVector(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
	DirectX::XMFLOAT3 box2X = box2.GetTransform()->GetRotation().RotateVector(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));
	DirectX::XMFLOAT3 box2Y = box2.GetTransform()->GetRotation().RotateVector(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
	DirectX::XMFLOAT3 box2Z = box2.GetTransform()->GetRotation().RotateVector(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));

	testAxis.push_back(DirectX::XMLoadFloat3(&box1X));
	testAxis.push_back(DirectX::XMLoadFloat3(&box1Y));
	testAxis.push_back(DirectX::XMLoadFloat3(&box1Z));
	testAxis.push_back(DirectX::XMLoadFloat3(&box2X));
	testAxis.push_back(DirectX::XMLoadFloat3(&box2Y));
	testAxis.push_back(DirectX::XMLoadFloat3(&box2Z));

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
		box1.GetTransform()->GetPosition().x - box2.GetTransform()->GetPosition().x,
		box1.GetTransform()->GetPosition().y - box2.GetTransform()->GetPosition().y,
		box1.GetTransform()->GetPosition().z - box2.GetTransform()->GetPosition().z);

	DirectX::XMVECTOR positionDifference = DirectX::XMLoadFloat3(&posDifference);

	for (unsigned i = 0; i < testAxis.size(); i++)
	{
		float distance = std::abs(
			DirectX::XMVectorGetByIndex(
				DirectX::XMVector3Dot(
					positionDifference,
					testAxis[i]),
				0));

		float leftSide = box1.GreatestPointAlongAxis(testAxis[i]);
		float rightSide = box2.GreatestPointAlongAxis(testAxis[i]);

		if (distance > leftSide + rightSide)
		{
			return false;
		}
	}

	return true;
}
