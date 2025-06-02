#pragma once

#define _USE_MATH_DEFINES

#include <DirectXMath.h>
#include <cmath>
#include <WICTextureLoader.h>
#include "DirectXGameCore.h"
#include "SimpleShader.h"
#include "Mesh.h"
#include "EntityCollision.h"
#include "Texture.h"
#include "Camera.h"
#include "Material.h"
#include "DirectionalLight.h"
#include "Collision.h"

// Include run-time memory checking in debug builds, so 
// we can be notified of memory leaks
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// --------------------------------------------------------
// Game class which extends the base DirectXGameCore class
// --------------------------------------------------------
class MyDemoGame : public DirectXGameCore
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float deltaTime, float totalTime);
	void DrawScene(float deltaTime, float totalTime);

	// For handing mouse input
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private:
	// Initialization for our "game" demo - Feel free to
	// expand, alter, rename or remove these once you
	// start doing something more advanced!
	void CreateGeometry();
	void CreateLights();
	void LoadShaders();
	void LoadTexturesAndMaterials();
	void CreateEntities();
	void CreateMatrices();

private:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<EntityCollision> entity1;
	std::shared_ptr<EntityCollision> entity2;
	std::shared_ptr<EntityCollision> entity3;
	std::shared_ptr<Material> material1;
	std::shared_ptr<Material> material2;
	std::shared_ptr<Material> material3;
	std::shared_ptr<Texture> tex;
	DirectionalLight light;
	Camera camera;


	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};