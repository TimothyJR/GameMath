#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition - You will eventually ADD TO this!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 position;		// The position of the vertex
	//DirectX::XMFLOAT4 Color;			// The color of the vertex
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
};