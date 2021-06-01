#include "Entity.h"

Entity::Entity(Mesh* m, Material* mat)
{
	mesh = m;
	material = mat;
	transform = Transform();
}

Entity::~Entity()
{
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix()
{
	return transform.GetWorldMatrix();
}

Transform Entity::GetTransform()
{
	return transform;
}

void Entity::SetTransform(Transform& t)
{
	transform = t;
}

void Entity::SetPosition(DirectX::XMFLOAT3 p)
{
	transform.SetPosition(p);
}

void Entity::SetRotation(Quaternion& q)
{
	transform.SetRotation(q);
}

void Entity::SetScale(DirectX::XMFLOAT3 s)
{
	transform.SetScale(s);
}

void Entity::SetMaterial(Material* mat)
{
	material = mat;
}

void Entity::Draw(ID3D11DeviceContext* dc, Camera& camera, DirectionalLight& light)
{
	material->Draw(camera, light, GetWorldMatrix());
	mesh->Draw(dc);
}