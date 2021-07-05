#pragma once
#define _USE_MATH_DEFINES

#include <DirectXMath.h>
#include <cmath>

class Quaternion
{
public:
	Quaternion();
	Quaternion(const DirectX::XMFLOAT4&);
	Quaternion(const DirectX::XMFLOAT3&, float);
	Quaternion(float _x, float _y, float _z, float _w);

	Quaternion Multiply(const Quaternion& q) const;
	Quaternion Slerp(const Quaternion& q, float t) const;

	DirectX::XMFLOAT3 RotateVector(const DirectX::XMFLOAT3&);
	DirectX::XMFLOAT3 ToEuler();

	static Quaternion FromEuler(float, float, float);
	static Quaternion LookAt(const DirectX::XMFLOAT3&);
	static Quaternion LookAt(const DirectX::XMFLOAT3&, const DirectX::XMFLOAT3&);

	float x;
	float y;
	float z;
	float w;
private:
	Quaternion Inverse() const;
	Quaternion QuaternionScaledToT(const DirectX::XMFLOAT3&, float, float) const;
};