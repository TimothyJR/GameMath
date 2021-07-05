#include "EntityCollision.h"

EntityCollision::EntityCollision(const std::shared_ptr<Mesh>& m, const std::shared_ptr<Material>& mat)
	: Entity(m, mat), collider(BoxCollider(*m, transform))
{
}

BoxCollider& EntityCollision::GetBoxCollider()
{
	return collider;
}