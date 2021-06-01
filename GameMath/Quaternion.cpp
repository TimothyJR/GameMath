#include "Quaternion.h"

Quaternion::Quaternion() :
	Quaternion(0, 0, 0, 1)
{
}

Quaternion::Quaternion(DirectX::XMFLOAT4& q) :
	Quaternion(q.x, q.y, q.z, q.w)
{
}

Quaternion::Quaternion(DirectX::XMFLOAT3& v, float a)
{
	float s = sin(a / 2.0f);

	DirectX::XMVECTOR vector = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&v));

	x = DirectX::XMVectorGetByIndex(vector, 0) * s;
	y = DirectX::XMVectorGetByIndex(vector, 1) * s;
	z = DirectX::XMVectorGetByIndex(vector, 2) * s;
	w = cos(a / 2.0f);
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w) :
	x (_x), y (_y), z (_z), w (_w)
{
}

Quaternion Quaternion::Multiply(Quaternion& q)
{
	DirectX::XMVECTOR v = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z));
	DirectX::XMVECTOR qv = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(q.x, q.y, q.z));

	DirectX::XMFLOAT3 cross;
	DirectX::XMStoreFloat3(&cross, DirectX::XMVector3Cross(v, qv));

	float dot = DirectX::XMVectorGetByIndex(DirectX::XMVector3Dot(v, qv), 0);

	Quaternion newQ = Quaternion();

	newQ.w = q.w * w - dot;
	newQ.x = q.w * x + w * q.x + cross.x;
	newQ.y = q.w * y + w * q.y + cross.y;
	newQ.z = q.w * z + w * q.z + cross.z;

	return newQ;
}

DirectX::XMFLOAT3 Quaternion::RotateVector(DirectX::XMFLOAT3& v)
{
	DirectX::XMFLOAT3 u = {x, y, z};

	DirectX::XMFLOAT3 cross;

	DirectX::XMStoreFloat3(&cross,
		DirectX::XMVector3Cross(
			DirectX::XMLoadFloat3(&u),
			DirectX::XMLoadFloat3(&v)
		));

	float dotuv = DirectX::XMVectorGetByIndex(
		DirectX::XMVector3Dot(
			DirectX::XMLoadFloat3(&u),
			DirectX::XMLoadFloat3(&v)),
		0);

	float dotuu = DirectX::XMVectorGetByIndex(
		DirectX::XMVector3Dot(
			DirectX::XMLoadFloat3(&u),
			DirectX::XMLoadFloat3(&u)),
		0);


	DirectX::XMFLOAT3 returnVector =
	{
		2.0f * dotuv * u.x + (w * w - dotuu) * v.x + 2.0f * w * cross.x,
		2.0f * dotuv * u.y + (w * w - dotuu) * v.y + 2.0f * w * cross.y,
		2.0f * dotuv * u.z + (w * w - dotuu) * v.z + 2.0f * w * cross.z
	};

	return returnVector;
}

DirectX::XMFLOAT3 Quaternion::ToEuler()
{
	DirectX::XMFLOAT3 euler;

	float sinRollCosPitch = 2 * (w * x + y * z);
	float cosRollCosPitch = 1 - 2 * (x * x + y * y);
	euler.x = atan2f(sinRollCosPitch, cosRollCosPitch);

	float sinPitch = 2 * (w * y - z * x);
	if (fabsf(sinPitch >= 1))
	{
		euler.y = copysignf(M_PI / 2, sinPitch);
	}
	else
	{
		euler.y = asinf(sinPitch);
	}

	float sinYawCosPitch = 2 * (w * z + x * y);
	float cosYawCosPitch = 1 - 2 * (y * y + z * z);
	euler.z = atan2f(sinYawCosPitch, cosYawCosPitch);

	return euler;
}

Quaternion Quaternion::Slerp(Quaternion& q, float t)
{
	//Quaternion returnValue = q.Multiply(this->Inverse());
	//returnValue = QuaternionScaledToT({ returnValue.x, returnValue.y, returnValue.z }, returnValue.w, t).Multiply(*this);
	//return returnValue;

	float dot = DirectX::XMVectorGetByIndex(
		DirectX::XMVector3Dot(
			DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(x, y, z)),
			DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(q.x, q.y, q.z))),
		0);

	float cosOmega = w * q.w + dot;

	if (cosOmega < 0)
	{
		q.w = -q.w;
		q.x = -q.x;
		q.y = -q.y;
		q.z = -q.z;
		cosOmega = -cosOmega;
	}

	float k0;
	float k1;

	if (cosOmega > 0.9999f)
	{
		k0 = 1 - t;
		k1 = t;
	}
	else
	{
		float sinOmega = sqrtf(1 - cosOmega * cosOmega);

		float omega = atan2f(sinOmega, cosOmega);
		float oneOver = 1 / sinOmega;

		k0 = sin((1 - t) * omega) * oneOver;
		k1 = sin(t * omega) * oneOver;
	}

	Quaternion returnQuaternion = Quaternion();

	returnQuaternion.w = w * k0 + q.w * k1;
	returnQuaternion.x = x * k0 + q.x * k1;
	returnQuaternion.y = y * k0 + q.y * k1;
	returnQuaternion.z = z * k0 + q.z * k1;

	return returnQuaternion;
}

Quaternion Quaternion::FromEuler(float roll, float pitch, float yaw)
{
	float cosRoll = cosf(roll * 0.5f);
	float cosPitch = cosf(pitch * 0.5f);
	float cosYaw = cosf(yaw * 0.5f);

	float sinRoll = sinf(roll * 0.5f);
	float sinPitch = sinf(pitch * 0.5f);
	float sinYaw = sinf(yaw * 0.5f);

	return Quaternion(
		sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw,
		cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
		cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
		cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw
	);
}

Quaternion Quaternion::LookAt(DirectX::XMFLOAT3& direction)
{
	return LookAt(direction, DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
}

Quaternion Quaternion::LookAt(DirectX::XMFLOAT3& direction, DirectX::XMFLOAT3& up)
{
	DirectX::XMVECTOR lookAtV = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));
	DirectX::XMVECTOR rightV = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&up), lookAtV));
	DirectX::XMVECTOR rotatedUpV = DirectX::XMVector3Cross(lookAtV, rightV);

	DirectX::XMFLOAT3 lookAt;
	DirectX::XMFLOAT3 right;
	DirectX::XMFLOAT3 angleUp;

	DirectX::XMStoreFloat3(&lookAt, lookAtV);
	DirectX::XMStoreFloat3(&right, rightV);
	DirectX::XMStoreFloat3(&angleUp, rotatedUpV);

	Quaternion q;
	float t = right.x + angleUp.y + lookAt.z;

	if (t > 0.0f)
	{
		float s = 0.5f / sqrtf(t + 1.0f);
		q.w = 0.25f / s;
		q.x = (angleUp.z - lookAt.y) * s;
		q.y = (lookAt.x - right.z) * s;
		q.z = (right.y - angleUp.x) * s;
	}
	else
	{
		if (right.x > angleUp.y && right.x > lookAt.z)
		{
			float s = 2.0f * sqrtf(1.0f + right.x - angleUp.y - lookAt.z);
			q.w = (angleUp.z - lookAt.y) / s;
			q.x = 0.25f * s;
			q.y = (angleUp.x + right.y) / s;
			q.z = (lookAt.x + right.z) / s;
		}
		else if (angleUp.y > lookAt.z)
		{
			float s = 2.0f * sqrtf(1.0 + angleUp.y - right.x - lookAt.z);
			q.w = (lookAt.x - right.z) / s;
			q.x = (angleUp.x + right.y) / s;
			q.y = 0.25f * s;
			q.z = (lookAt.y + angleUp.z) / s;
		}
		else
		{
			float s = 2.0f * sqrtf(1.0 + lookAt.z - right.x - angleUp.y);
			q.w = (right.y - angleUp.x) / s;
			q.x = (lookAt.x + right.z) / s;
			q.y = (lookAt.y + angleUp.z) / s;
			q.z = 0.25f * s;
		}
	}

	return q;

	// OLD - Does not respect roll
	//DirectX::XMFLOAT3 forward = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	//DirectX::XMVECTOR dir = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));
	//
	//DirectX::XMVECTOR forw = DirectX::XMLoadFloat3(&forward);
	//
	//float dot = DirectX::XMVectorGetByIndex(DirectX::XMVector3Dot(dir, forw), 0);
	//
	//if (abs(dot + 1) < 0.000001f)
	//{
	//	// Opposite directions, rotate entirely around
	//	return Quaternion(up, M_PI);
	//}
	//
	//if (abs(dot - 1) < 0.000001f)
	//{
	//	// Same direction, no need to change rotation
	//	return Quaternion();
	//}
	//
	//float rotationAngle = acosf(dot);
	//
	//DirectX::XMFLOAT3 rotationAxis;
	//DirectX::XMStoreFloat3(&rotationAxis, DirectX::XMVector3Cross(forw, dir));
	//
	//return Quaternion(rotationAxis, rotationAngle);
}

Quaternion Quaternion::Inverse()
{
	Quaternion(-x, -y, -z, w);
	return Quaternion();
}

Quaternion Quaternion::QuaternionScaledToT(DirectX::XMFLOAT3& v, float a, float t)
{
	DirectX::XMFLOAT3 returnVector;
	DirectX::XMVECTOR vector = DirectX::XMLoadFloat3(&v);

	if (DirectX::XMVectorGetByIndex(DirectX::XMVector3LengthSq(vector), 0) < 0.0001f)
	{
		returnVector = { 1, 0, 0 };
	}
	else
	{
		vector = DirectX::XMVector3Normalize(vector);
		DirectX::XMStoreFloat3(&returnVector, vector);
	}

	float angle = acosf(w) * 2;

	return Quaternion(returnVector, angle * t);
}
