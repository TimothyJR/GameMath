#include "Entity.h"

Entity::Entity(const Mesh* m, const Material* mat) :
	mesh(m), material(mat)
{
	transform = std::make_shared<Transform>();
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix()
{
	return transform->GetWorldMatrix();
}

DirectX::XMFLOAT4X4 Entity::GetWorldMatrix() const
{
	return transform->GetWorldMatrix();
}

std::shared_ptr<Transform> Entity::GetTransform() const
{
	return transform;
}

void Entity::SetTransform(Transform& t)
{
	transform.reset(&t);
}

void Entity::SetPosition(DirectX::XMFLOAT3 p)
{
	transform->SetPosition(p);
}

void Entity::SetRotation(Quaternion&& q)
{
	transform->SetRotation(q);
}

void Entity::SetScale(DirectX::XMFLOAT3 s)
{
	transform->SetScale(s);
}

void Entity::SetMaterial(Material& mat)
{
	material.reset(&mat);
}

void Entity::Draw(ID3D11DeviceContext* dc, const Camera& camera, const DirectionalLight& light) const
{
	material->Draw(camera, light, GetWorldMatrix());
	mesh->Draw(dc);
}