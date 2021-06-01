#include "EntityCollision.h"

EntityCollision::EntityCollision(Mesh* m, Material* mat)
	: Entity(m, mat), collider(BoxCollider(*m, transform))
{
}

BoxCollider& EntityCollision::GetBoxCollider()
{
	return collider;
}