#include "Mesh.h"
#include <fstream>

Mesh::Mesh(Vertex* vertices, int numVerts, unsigned* indices, int numIndices, ID3D11Device& device)
{
	InitializeMinMax();

	for (int i = 0; i < numVerts; i++)
	{
		CheckMinMax(vertices[i].position);
	}

	CalculateCentroid();
	CreateVertexBuffer(vertices, numVerts, device);
	CreateIndexBuffer(indices, numIndices, device);
	numberOfIndices = numIndices;
}

Mesh::Mesh(const char* filePath, ID3D11Device& device)
{
	InitializeMinMax();

	// File input object
	std::ifstream obj(filePath);

	// Check for successful open
	if (!obj.is_open())
		return;

	// Variables used while reading the file
	std::vector<DirectX::XMFLOAT3> positions;     // Positions from the file
	std::vector<DirectX::XMFLOAT3> normals;       // Normals from the file
	std::vector<DirectX::XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int triangleCounter = 0;    // Count of triangles/indices
	char chars[100];                     // String for line reading

	// Still good?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			DirectX::XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			CheckMinMax(pos);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the 9 face indices into an array
			unsigned int i[9];
			sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.position = positions[i[0] - 1];
			v1.uv = uvs[i[1] - 1];
			v1.normal = normals[i[2] - 1];

			Vertex v2;
			v2.position = positions[i[3] - 1];
			v2.uv = uvs[i[4] - 1];
			v2.normal = normals[i[5] - 1];

			Vertex v3;
			v3.position = positions[i[6] - 1];
			v3.uv = uvs[i[7] - 1];
			v3.normal = normals[i[8] - 1];

			// Add the verts to the vector
			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);

			// Add three more indices
			indices.push_back(triangleCounter++);
			indices.push_back(triangleCounter++);
			indices.push_back(triangleCounter++);
		}
	}

	CalculateCentroid();

	// Close
	obj.close();

	CreateVertexBuffer(Helpers::copyVectorToArrayShallow(verts).get(), verts.size(), device);
	CreateIndexBuffer(Helpers::copyVectorToArrayShallow(indices).get(), indices.size(), device);
	numberOfIndices = indices.size();
}

Mesh::~Mesh()
{
	vertexBuffer->Release();
	indexBuffer->Release();
}

void Mesh::Draw(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(numberOfIndices, 0, 0);
}

ID3D11Buffer* Mesh::GetVertexBuffer()
{
	return vertexBuffer;
}

ID3D11Buffer* Mesh::GetIndexBuffer()
{
	return indexBuffer;
}

int Mesh::GetIndexCount()
{
	return numberOfIndices;
}

DirectX::XMFLOAT3 Mesh::GetCentroid()
{
	return centroid;
}

DirectX::XMFLOAT3 Mesh::GetMin()
{
	return min;
}

DirectX::XMFLOAT3 Mesh::GetMax()
{
	return max;
}

void Mesh::CreateVertexBuffer(Vertex* verts, int numVerts, ID3D11Device& device)
{
	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * numVerts;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	
	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = verts;
	
	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device.CreateBuffer(&vbd, &initialVertexData, &vertexBuffer));
}

void Mesh::CreateIndexBuffer(unsigned* indices, int numIndices, ID3D11Device& device)
{
	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * numIndices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	
	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;
	
	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	HR(device.CreateBuffer(&ibd, &initialIndexData, &indexBuffer));
}

void Mesh::InitializeMinMax()
{
	min =
	{
		FLT_MAX,
		FLT_MAX,
		FLT_MAX
	};

	max =
	{
		FLT_MIN,
		FLT_MIN,
		FLT_MIN
	};
}

void Mesh::CalculateCentroid()
{
	centroid =
	{
		(min.x + max.x) / 2,
		(min.y + max.y) / 2,
		(min.z + max.z) / 2
	};
}

void Mesh::CheckMinMax(const DirectX::XMFLOAT3& pos)
{
	// Check for min and max vertex position
	min.x = std::min(min.x, pos.x);
	max.x = std::max(max.x, pos.x);
	min.y = std::min(min.y, pos.y);
	max.y = std::max(max.y, pos.y);
	min.z = std::min(min.z, pos.z);
	max.z = std::max(max.z, pos.z);
}
