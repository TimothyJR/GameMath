#include "SphereCollider.h"

SphereCollider::SphereCollider(DirectX::XMFLOAT3& c, float r, const std::shared_ptr<Transform>& t) :
	center (c), radius (r), transform(t)
{
}

SphereCollider::SphereCollider(const Mesh& mesh, const std::shared_ptr<Transform>& t) :
	transform(t)
{
	center = mesh.GetCentroid();

	if (mesh.GetMax().x < mesh.GetMax().y)
	{
		if (mesh.GetMax().x < mesh.GetMax().z)
		{
			radius = mesh.GetMax().x;
		}
		else
		{
			radius = mesh.GetMax().z;
		}
	}
	else
	{
		if (mesh.GetMax().y > mesh.GetMax().z)
		{
			radius = mesh.GetMax().y;
		}
		else
		{
			radius = mesh.GetMax().z;
		}
	}

}

DirectX::XMFLOAT3 SphereCollider::GetCenter() const
{
	return center;
}

float SphereCollider::GetRadius() const
{
	return radius;
}

void SphereCollider::SetCenter(DirectX::XMFLOAT3& c)
{
	center = c;
}

void SphereCollider::SetRadius(float r)
{
	radius = r;
}

bool SphereCollider::CheckCollision(const SphereCollider& collider) const
{
	DirectX::XMFLOAT3 position =
	{
		center.x + transform->GetPosition().x,
		center.y + transform->GetPosition().y,
		center.z + transform->GetPosition().z
	};

	DirectX::XMFLOAT3 positionOther =
	{
		collider.center.x + collider.transform->GetPosition().x,
		collider.center.y + collider.transform->GetPosition().y,
		collider.center.z + collider.transform->GetPosition().z
	};

	float distance = DirectX::XMVectorGetByIndex(DirectX::XMVector3Length(
		DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&position),
			DirectX::XMLoadFloat3(&positionOther)
		)
	), 0);

	if (distance < radius + collider.radius)
	{
		return true;
	}

	return false;
}
