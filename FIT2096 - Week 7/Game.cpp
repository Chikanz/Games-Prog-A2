/*	FIT2096 - Example Code
*	Game.cpp
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*/

#include "Game.h"
#include "TexturedShader.h"
#include "StaticObject.h"

#include "FlyingCamera.h"
#include "ThirdPersonCamera.h"

#include "MathsHelper.h"
#include "DirectXTK/CommonStates.h"
#include <sstream>

Game::Game()
{
	m_renderer = NULL;
	m_currentCam = NULL;
	m_input = NULL;
	m_meshManager = NULL;
	m_textureManager = NULL;
	m_diffuseTexturedShader = NULL;
	m_unlitVertexColouredShader = NULL;
	m_spriteBatch = NULL;
	m_arialFont12 = NULL;
	m_arialFont18 = NULL;
	m_player = NULL;
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{
	m_renderer = renderer;	
	m_input = input;
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();

	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	LoadFonts();
	InitUI();
	InitGameWorld();
	RefreshUI();

	m_collisionManager = new CollisionManager(&m_karts, &m_itemBoxes);

	//m_currentCam = new FlyingCamera(m_input, Vector3(0, 10, -20));
	m_currentCam = new ThirdPersonCamera(m_player, Vector3(0, 10, -25), true, 2.0f);

	return true;
}

bool Game::InitShaders()
{
	m_unlitVertexColouredShader = new Shader();
	if (!m_unlitVertexColouredShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/UnlitVertexColourPixelShader.ps"))
		return false;

	m_unlitTexturedShader = new TexturedShader();
	if (!m_unlitTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/UnlitTexturedPixelShader.ps"))
		return false;

	m_diffuseTexturedShader = new TexturedShader();
	if (!m_diffuseTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedPixelShader.ps"))
		return false;

	m_diffuseTexturedFogShader = new TexturedShader();
	if (!m_diffuseTexturedFogShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedFogPixelShader.ps"))
		return false;

	return true;
}

bool Game::LoadMeshes()
{
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/kart.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ground.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/wall.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/rumble_strip.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/item_box.obj"))
		return false;

	return true;
}

bool Game::LoadTextures()
{
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/kart_red.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/kart_green.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/kart_blue.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/kart_purple.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/kart_orange.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/grass.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/rumble_strip.jpg"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/wall.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/item_box.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/button.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_star.png"))
		return false;

	return true;
}

void Game::LoadFonts()
{
	// There's a few different size fonts in there, you know
	m_arialFont12 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-12pt.spritefont");
	m_arialFont18 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-18pt.spritefont");
}

void Game::InitUI()
{
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext());
	m_currentItemSprite = m_textureManager->GetTexture("Assets/Textures/sprite_star.png");

	// Also init any buttons here
}

void Game::RefreshUI()
{
	// Ensure text in UI matches latest scores etc (call this after data changes)
}

void Game::InitGameWorld()
{
	InitKarts();
	InitItemBoxes();

	// Static scenery objects
	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/grass.jpg")));

	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/wall.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/wall.png")));

	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/rumble_strip.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/rumble_strip.jpg")));

}

void Game::InitKarts()
{
	m_player = new Kart(m_meshManager->GetMesh("Assets/Meshes/kart.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/kart_red.png"),
		Vector3(0, 0, -10),
		m_input);

	m_gameObjects.push_back(m_player);
	m_karts.push_back(m_player);
}

void Game::InitItemBoxes()
{
	for (int i = 0; i < 20; i++)
	{
		Vector3 position = Vector3(MathsHelper::RandomRange(-200.0f, 200.0f), 0.0f, MathsHelper::RandomRange(-200.0f, 200.0f));

		ItemBox* itemBox = new ItemBox(m_meshManager->GetMesh("Assets/Meshes/item_box.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture("Assets/Textures/item_box.png"),
			position);
		
		m_itemBoxes.push_back(itemBox);
		m_gameObjects.push_back(itemBox);
	}
}

void Game::Update(float timestep)
{
	m_input->BeginUpdate();

	if (m_input->GetKeyDown(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}
	
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(timestep);
	}

	m_collisionManager->CheckCollisions();

	// Exercise two (before we use the collison manager)
	/*CBoundingBox playerBounds = m_player->GetBounds();

	for (unsigned int i = 0; i < m_itemBoxes.size(); i++)
	{
		CBoundingBox itemBoxBounds = m_itemBoxes[i]->GetBounds();
		if (CheckCollision(playerBounds, itemBoxBounds))
		{
			OutputDebugString("Kart-ItemBox Collision\n");
		}
	}*/

	m_currentCam->Update(timestep);

	m_input->EndUpdate();
}

void Game::Render()
{
	m_renderer->BeginScene(0.8f, 1.0f, 0.9f, 1.0f);

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Render(m_renderer, m_currentCam);
	}

	DrawUI();

	m_renderer->EndScene();		
}

void Game::DrawUI()
{
	// Sprites don't use a shader 
	m_renderer->SetCurrentShader(NULL);

	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	// Do UI drawing between the Begin and End calls

	// Let's draw some text over our game
	m_arialFont18->DrawString(m_spriteBatch, L"ESC to quit", Vector2(20, 160), Color(0.0f, 0.0f, 0.0f), 0, Vector2(0,0));
	
	// Here's how we draw a sprite over our game (caching it in m_currentItemSprite so we don't have to find it every frame)
	m_spriteBatch->Draw(m_currentItemSprite->GetShaderResourceView(), Vector2(20, 20), Color(1.0f, 1.0f, 1.0f));
	
	m_spriteBatch->End();
}

void Game::Shutdown()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}

	m_gameObjects.empty();

	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_unlitVertexColouredShader)
	{
		m_unlitVertexColouredShader->Release();
		delete m_unlitVertexColouredShader;
		m_unlitVertexColouredShader = NULL;
	}

	if (m_unlitTexturedShader)
	{
		m_unlitTexturedShader->Release();
		delete m_unlitTexturedShader;
		m_unlitTexturedShader = NULL;
	}

	if(m_diffuseTexturedShader)
	{
		m_diffuseTexturedShader->Release();
		delete m_diffuseTexturedShader;
		m_diffuseTexturedShader = NULL;
	}

	if (m_diffuseTexturedFogShader)
	{
		m_diffuseTexturedFogShader->Release();
		delete m_diffuseTexturedShader;
		m_diffuseTexturedShader = NULL;
	}

	if (m_meshManager)
	{
		m_meshManager->Release();
		delete m_meshManager;
		m_meshManager = NULL;
	}

	if (m_textureManager)
	{
		m_textureManager->Release();
		delete m_textureManager;
		m_textureManager = NULL;
	}

	if (m_spriteBatch)
	{
		delete m_spriteBatch;
		m_spriteBatch = NULL;
	}

	if (m_arialFont12)
	{
		delete m_arialFont12;
		m_arialFont12 = NULL;
	}

	if (m_arialFont18)
	{
		delete m_arialFont18;
		m_arialFont18 = NULL;
	}
}