#pragma once
#include "Entity.h"
#include "BoxCollider.h"
class EntityCollision :
	public Entity
{
public:
	EntityCollision(const std::shared_ptr<Mesh>& m, const std::shared_ptr<Material>& mat);
	BoxCollider& GetBoxCollider();
private:
	BoxCollider collider;
};