#include "Collider.h"

Collider::Collider(Transform* t) :
	transform(t)
{
}

const Transform* Collider::GetTransform() const
{
	return transform.get();
}
