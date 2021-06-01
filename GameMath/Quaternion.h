#pragma once
#define _USE_MATH_DEFINES

#include <DirectXMath.h>
#include <cmath>

class Quaternion
{
public:
	Quaternion();
	Quaternion(DirectX::XMFLOAT4&);
	Quaternion(DirectX::XMFLOAT3&, float);
	Quaternion(float _x, float _y, float _z, float _w);

	Quaternion Multiply(Quaternion& q);
	Quaternion Slerp(Quaternion& q, float t);

	DirectX::XMFLOAT3 RotateVector(DirectX::XMFLOAT3&);
	DirectX::XMFLOAT3 ToEuler();

	static Quaternion FromEuler(float, float, float);
	static Quaternion LookAt(DirectX::XMFLOAT3&);
	static Quaternion LookAt(DirectX::XMFLOAT3&, DirectX::XMFLOAT3&);

	float x;
	float y;
	float z;
	float w;
private:
	Quaternion Inverse();
	Quaternion QuaternionScaledToT(DirectX::XMFLOAT3&, float, float);
};