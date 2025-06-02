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
	Entity(const Mesh*, const Material*);
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetWorldMatrix() const;
	std::shared_ptr<Transform> GetTransform() const;

	void SetTransform(Transform&);
	void SetPosition(DirectX::XMFLOAT3);
	void SetRotation(Quaternion&& q);
	void SetScale(DirectX::XMFLOAT3);
	void SetMaterial(Material&);

	void Draw(ID3D11DeviceContext*, const Camera&, const DirectionalLight&) const;

private:
	std::shared_ptr<const Mesh> mesh;
	std::shared_ptr<const Material> material;

protected:
	std::shared_ptr<Transform> transform;
};