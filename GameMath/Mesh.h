#pragma once
#define NOMINMAX
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "d3d11.h"
#include "Vertex.h"
#include "DirectXGameCore.h"
#include "Helpers.h"

class Mesh
{
public:
	Mesh(Vertex*, int, unsigned*, int, ID3D11Device&);
	Mesh(const char* filePath, ID3D11Device&);
	~Mesh();
	void Draw(ID3D11DeviceContext*);
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();
	DirectX::XMFLOAT3 GetCentroid();
	DirectX::XMFLOAT3 GetMin();
	DirectX::XMFLOAT3 GetMax();

private:
	void CreateVertexBuffer(Vertex* verts, int numVerts, ID3D11Device& device);
	void CreateIndexBuffer(unsigned* indices, int numIndices, ID3D11Device& device);
	void InitializeMinMax();
	void CalculateCentroid();
	void CheckMinMax(const DirectX::XMFLOAT3&);
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numberOfIndices;
	DirectX::XMFLOAT3 centroid;
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;

};