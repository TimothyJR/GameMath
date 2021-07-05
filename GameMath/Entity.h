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
	Entity(const std::shared_ptr<Mesh>&, const std::shared_ptr<Material>&);
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT4X4 GetWorldMatrix() const;
	std::shared_ptr<Transform> GetTransform() const;

	void SetTransform(Transform&);
	void SetPosition(DirectX::XMFLOAT3);
	void SetRotation(Quaternion& q);
	void SetScale(DirectX::XMFLOAT3);
	void SetMaterial(Material&);

	void Draw(ID3D11DeviceContext*, const Camera&, const DirectionalLight&);

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

protected:
	std::shared_ptr<Transform> transform;
};