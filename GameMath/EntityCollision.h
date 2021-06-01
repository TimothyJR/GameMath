#pragma once
#include "Entity.h"
#include "BoxCollider.h"
class EntityCollision :
	public Entity
{
public:
	EntityCollision(Mesh* m, Material* mat);
	BoxCollider& GetBoxCollider();
private:
	BoxCollider collider;
};