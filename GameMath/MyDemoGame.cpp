// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------

#include "MyDemoGame.h"
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;


#pragma region Win32 Entry Point (WinMain)
// --------------------------------------------------------
// Win32 Entry Point - Where your program starts
// --------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance,
						_In_ PSTR cmdLine, _In_ int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	// Create the game object.
	MyDemoGame game(hInstance);
	
	// This is where we'll create the window, initialize DirectX, 
	// set up geometry and shaders, etc.
	if( !game.Init() )
		return 0;
	
	// All set to run the game loop
	return game.Run();
}

#pragma endregion

#pragma region Constructor / Destructor
// --------------------------------------------------------
// Base class constructor will set up all of the underlying
// fields, and then we can overwrite any that we'd like
// --------------------------------------------------------
MyDemoGame::MyDemoGame(HINSTANCE hInstance) 
	: DirectXGameCore(hInstance)
{
	// Set up a custom caption for the game window.
	// - The "L" before the string signifies a "wide character" string
	// - "Wide" characters take up more space in memory (hence the name)
	// - This allows for an extended character set (more fancy letters/symbols)
	// - Lots of Windows functions want "wide characters", so we use the "L"
	windowCaption = L"Game Math Project";

	// Custom window size - will be created by Init() later
	windowWidth = 800;
	windowHeight = 600;
}

// --------------------------------------------------------
// Cleans up our DirectX stuff and any objects we need to delete
// - When you make new DX resources, you need to release them here
// - If you don't, you get a lot of scary looking messages in Visual Studio
// --------------------------------------------------------
MyDemoGame::~MyDemoGame()
{
}

#pragma endregion

#pragma region Initialization

// --------------------------------------------------------
// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
// --------------------------------------------------------
bool MyDemoGame::Init()
{
	// Call the base class's Init() method to create the window,
	// initialize DirectX, etc.
	if( !DirectXGameCore::Init() )
		return false;

	camera = Camera();


	// Helper methods to create something to draw, load shaders to draw it 
	// with and set up matrices so we can see how to pass data to the GPU.
	//  - For your own projects, feel free to expand/replace these.
	CreateGeometry();
	CreateLights();
	LoadShaders();
	LoadTexturesAndMaterials();
	CreateEntities();
	CreateMatrices();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives we'll be using and how to interpret them
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Successfully initialized
	return true;
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void MyDemoGame::CreateGeometry()
{
	//Vertex vertices[] = 
	//{
	//	{ XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
	//	{ XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
	//	{ XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
	//	{ XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
	//	{ XMFLOAT3(0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	//	{ XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	//	{ XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	//	{ XMFLOAT3(0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f) },
	//};
	//
	//unsigned indices[] = 
	//{
	//		1, 2, 0, // -x
	//		3, 2, 1,
	//
	//		6, 5, 4, // +x
	//		6, 7, 5,
	//
	//		5, 1, 0, // -y
	//		4, 5, 0,
	//
	//		7, 6, 2, // +y
	//		3, 7, 2,
	//
	//		6, 4, 0, // -z
	//		2, 6, 0,
	//
	//		7, 3, 1, // +z
	//		5, 7, 1,
	//};

	//mesh = new Mesh(vertices, 8, indices, 36, *device);
	mesh = std::make_shared<Mesh>(R"(Meshes\Cube.obj)", *device);
}

void MyDemoGame::CreateLights()
{
	light =
	{
		XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f),
		XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		XMFLOAT3(1.0, -1.0f, 1.0f)
	};
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// - These simple shaders provide helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void MyDemoGame::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, deviceContext);
	vertexShader->LoadShaderFile(L"VertexShader.cso");
	
	pixelShader = new SimplePixelShader(device, deviceContext);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

void MyDemoGame::LoadTexturesAndMaterials()
{
	//tex = std::make_shared<Texture>(Texture(*device, *deviceContext, L"bricks.jpg"));
	tex.reset(new Texture(*device, *deviceContext, L"bricks.jpg"));
	material1 = std::make_shared<Material>(vertexShader, pixelShader, tex);
	material1->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	
	material2 = std::make_shared<Material>(vertexShader, pixelShader, tex);
	material2->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	
	material3 = std::make_shared<Material>(vertexShader, pixelShader, tex);
	material3->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}

void MyDemoGame::CreateEntities()
{
	entity1 = std::make_shared<EntityCollision>(mesh, material1);
	entity2 = std::make_shared<EntityCollision>(mesh, material2);
	entity3 = std::make_shared<EntityCollision>(mesh, material3);
}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void MyDemoGame::CreateMatrices()
{
	camera.CalculateProjection(aspectRatio);
}

#pragma endregion

#pragma region Window Resizing

// --------------------------------------------------------
// Handles resizing DirectX "stuff" to match the (usually) new
// window size and updating our projection matrix to match
// --------------------------------------------------------
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGameCore::OnResize();

	camera.CalculateProjection(aspectRatio);
}
#pragma endregion

#pragma region Game Loop

// --------------------------------------------------------
// Update your game here - take input, move objects, etc.
// --------------------------------------------------------
float x = 0;
void MyDemoGame::UpdateScene(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		Quit();
	}

	camera.Update(deltaTime);
	entity1->SetPosition(DirectX::XMFLOAT3(sinf(totalTime) * 3, 0.0f, 0.0f));
	entity2->SetPosition(DirectX::XMFLOAT3(sinf(totalTime) * 3, cosf(totalTime) * 3, 0.0f));
	
	entity1->SetRotation(Quaternion(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), M_PI * cosf(totalTime)));
	entity2->SetRotation(Quaternion(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), M_PI * sinf(totalTime)));
	entity3->SetRotation(Quaternion(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), M_PI * sinf(totalTime)));
	
	entity1->GetBoxCollider().GenerateAABB();
	entity2->GetBoxCollider().GenerateAABB();
	
	material1->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	material2->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	material3->SetColor(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	
	if (entity1->GetBoxCollider().CheckCollision(entity2->GetBoxCollider()))
	{
		material1->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		material2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	}
	
	if (entity1->GetBoxCollider().CheckCollision(entity3->GetBoxCollider()))
	{
		material1->SetColor(DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		material3->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	}
	
	if (entity2->GetBoxCollider().CheckCollision(entity3->GetBoxCollider()))
	{
		material2->SetColor(DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		material3->SetColor(DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
	}

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void MyDemoGame::DrawScene(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of DrawScene (before drawing *anything*)
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	entity1->Draw(deviceContext, camera, light);
	entity2->Draw(deviceContext, camera, light);
	entity3->Draw(deviceContext, camera, light);
	// Present the buffer
	//  - Puts the image we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME
	//  - Always at the very end of the frame
	HR(swapChain->Present(0, 0));
}

#pragma endregion

#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseDown(WPARAM btnState, int x, int y)
{
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hMainWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseUp(WPARAM btnState, int x, int y)
{
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
//
// Feel free to add code to this method
// --------------------------------------------------------
void MyDemoGame::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (btnState & 0x0002)
	{
		/* Right button is down */
		camera.UpdateRotation(x - prevMousePos.x, y - prevMousePos.y);
	}
	//if (btnState & 0x0001) { /* Left button is down */ }
	//if (btnState & 0x0010) { /* Middle button is down */ }

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

}
#pragma endregion