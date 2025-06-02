#include "SphereCollider.h"

SphereCollider::SphereCollider(DirectX::XMFLOAT3& c, float r, Transform* t) :
	Collider(t), center (c), radius (r)
{
}

SphereCollider::SphereCollider(const Mesh& mesh, Transform* t) :
	Collider(t)
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

DirectX::XMFLOAT3 SphereCollider::GetWorldCenter() const
{
	return
	{
		center.x + transform->GetPosition().x,
		center.y + transform->GetPosition().y,
		center.z + transform->GetPosition().z
	};
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
