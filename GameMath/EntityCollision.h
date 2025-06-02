#pragma once
#include "Entity.h"
#include "BoxCollider.h"
class EntityCollision :
	public Entity
{
public:
	EntityCollision(const Mesh* m, const Material* mat);
	BoxCollider& GetBoxCollider();
private:
	BoxCollider collider;
};