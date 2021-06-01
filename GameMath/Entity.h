#pragma once
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "Quaternion.h"
#include "Transform.h"
class Entity
{
public:
	Entity(Mesh*, Material*);
	~Entity();
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	Transform GetTransform();

	void SetTransform(Transform&);
	void SetPosition(DirectX::XMFLOAT3);
	void SetRotation(Quaternion& q);
	void SetScale(DirectX::XMFLOAT3);
	void SetMaterial(Material*);

	void Draw(ID3D11DeviceContext*, Camera&, DirectionalLight&);

private:
	Mesh* mesh;
	Material* material;

protected:
	Transform transform;
};