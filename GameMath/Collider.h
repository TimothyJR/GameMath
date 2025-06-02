#pragma once

#include "Transform.h"
#include <memory>

class Collider
{
public:
	Collider(Transform* t);
	const Transform* GetTransform() const;
protected:
	std::shared_ptr<Transform> transform;
};