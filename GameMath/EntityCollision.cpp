#include "EntityCollision.h"

EntityCollision::EntityCollision(const Mesh* m, const Material* mat)
	: Entity(m, mat), collider(BoxCollider(*m, transform.get()))
{
}

BoxCollider& EntityCollision::GetBoxCollider()
{
	return collider;
}