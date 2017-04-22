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
#include "AmmoBox.h"

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

	m_collisionManager = new CollisionManager(m_player, &m_gameObjects);

	//m_currentCam = new FlyingCamera(m_input, Vector3(0, 1.5f, -20));
	m_player = new Player(m_input, Vector3(0, 1.5f, -20));
	m_currentCam = m_player;

	//m_currentCam = new ThirdPersonCamera(m_player, Vector3(0, 10, -25), true, 2.0f);

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
	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ammoBlock.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/bullet.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/enemy.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ground.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ruby.obj"))
		return false;

	return true;
}

bool Game::LoadTextures()
{
	if (!m_textureManager->Load(m_renderer, "Assets/Textures/bullet.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_red.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ground.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_hurtOverlay.png"))
		return false;

	return true;
}

void Game::LoadFonts()
{
	// There's a few different size fonts in there, you know
	m_arialFont12 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-12pt.spritefont");
	m_arialFont18 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-18pt.spritefont");
	m_arialFont23 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-23pt.spritefont");
}

void Game::InitUI()
{
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext());
	//m_currentItemSprite = m_textureManager->GetTexture("Assets/Textures/sprite_star.png");

	// Also init any buttons here
}

void Game::RefreshUI()
{
	if (m_player != nullptr)
	{
		std::wstringstream ss;
		ss << m_player->getInClip() << "/" << m_player->getAmmo();
		m_ammoText = ss.str();
	}
}

void Game::InitGameWorld()
{
	// Static scenery objects
	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ground.png")));

	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
		m_diffuseTexturedShader,
		m_textureManager->GetTexture("Assets/Textures/gradient_red.png")));

	m_gameObjects.push_back(new AmmoBox(m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
		m_diffuseTexturedShader,
		m_textureManager->GetTexture("Assets/Textures/gradient_red.png"),
		Vector3(1,0,1)));
}


void Game::Update(float timestep)
{
	m_input->BeginUpdate();

	RefreshUI();

	m_simTime = m_player->getSimSpeed();

	if (m_input->GetKeyDown(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	//Spawn bullets
	if(m_input->GetMouseUp(0) && m_player->canFire())
	{
		Bullet* b = m_player->SpawnBullet(
			m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
			m_diffuseTexturedShader,
			m_textureManager->GetTexture("Assets/Textures/bullet.png")
		);

		m_gameObjects.push_back(b);
	}
	
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(timestep, m_simTime);	
	}

	for (unsigned int i = 0; i < m_gameObjects.size(); i++) 
	{
		//Destroy if marked
		//Wasn't game to put in in the other array for fear of resizing issues
		if (m_gameObjects[i]->MarkedForDestroy())
		{
			m_gameObjects.erase(m_gameObjects.begin() + i);
		}
	}
	

	m_collisionManager->CheckCollisions();

	m_currentCam->Update(timestep);

	m_input->EndUpdate();
}

void Game::Render()
{
	m_renderer->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

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

	m_arialFont23->DrawString(m_spriteBatch, m_ammoText.c_str(), Vector2(20, 650), Color(1, 1, 1), 0, Vector2(0, 0),Vector2(2,2),SpriteEffects_None,1);
	
	// Here's how we draw a sprite over our game (caching it in m_currentItemSprite so we don't have to find it every frame)
	//m_spriteBatch->Draw(m_currentItemSprite->GetShaderResourceView(), Vector2(20, 20), Color(1.0f, 1.0f, 1.0f));
	
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