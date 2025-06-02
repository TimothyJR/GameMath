#pragma once
#include <DirectXMath.h>
#include <vector>
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "Transform.h"

class BoxCollider;
class SphereCollider;

class Collision
{
public:
	static bool CheckCollision(const BoxCollider& box1, const BoxCollider& box2);
	static bool CheckCollision(const SphereCollider& sphere1, const SphereCollider& sphere2);
	static bool CheckCollision(const BoxCollider& box, const SphereCollider& sphere);
	static bool CheckCollision(const SphereCollider& sphere, const BoxCollider& box);

private:
	static bool SeparateAxisTest(const BoxCollider& box1, const BoxCollider& box2);
};