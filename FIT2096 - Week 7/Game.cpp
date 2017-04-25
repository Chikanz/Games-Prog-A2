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
#include "Agent2.h"

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

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/gradient_redPink.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/ground.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_healthBar.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/sprite_hurtOverlay.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/crosshair.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/pedestal.png"))
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
	m_crossHair = m_textureManager->GetTexture("Assets/Textures/crosshair.png");
	m_hurtOverlay = m_textureManager->GetTexture("Assets/Textures/sprite_hurtOverlay.png");
	m_healthBar = m_textureManager->GetTexture("Assets/Textures/sprite_healthBar.png");

	// Also init any buttons here
}

void Game::RefreshUI()
{
	if (m_player != nullptr)
	{
		std::wstringstream ss;
		ss << m_player->getInClip() << "/" << m_player->getAmmo();
		m_ammoText = ss.str();

		std::wstringstream ss2;
		ss2 << m_player->GetRubiesHeld() << "/" << m_player->GetMaxRubies();
		m_rubyText = ss2.str();
	}
}

void Game::InitGameWorld()
{
	//Player
	m_player = new Player(m_input, Vector3(0, 1.5f, -20), m_meshManager->GetMesh("Assets/Meshes/enemy.obj"));
	m_currentCam = m_player;

	//Collision manager
	GameObject* dummyPlayer = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
		m_diffuseTexturedShader,
		m_textureManager->GetTexture("Assets/Textures/gradient_red.png"));
	dummyPlayer->SetTag("Player");

	m_collisionManager = new CollisionManager(m_player, dummyPlayer, &m_gameObjects);


	// Static scenery objects
	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ground.png")));

	//m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
	//	m_diffuseTexturedShader,
	//	m_textureManager->GetTexture("Assets/Textures/gradient_red.png")));
	//m_gameObjects[1]->SetTag("Level");


	//Spawn enemies
	Enemy* e1 = new Agent2(m_input,
		m_player,
		m_meshManager->GetMesh("Assets/Meshes/enemy.obj"),
		m_diffuseTexturedShader,
		m_textureManager->GetTexture("Assets/Textures/gradient_red.png"),
		Vector3(0, 0, 0));
	m_gameObjects.push_back(e1);
	m_enemies.push_back(e1);


	//Spawn ammo boxes
	for(int i = 0; i < 10; i++)
	{
		float x = MathsHelper::RandomRange(-20, 20);
		float z = MathsHelper::RandomRange(-20, 20);

		m_gameObjects.push_back(new AmmoBox(m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
			m_diffuseTexturedShader,
			m_textureManager->GetTexture("Assets/Textures/gradient_red.png"),
			Vector3(x, 1, z)));

		//Pedestal
		StaticObject * s = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
			m_diffuseTexturedShader,
			m_textureManager->GetTexture("Assets/Textures/pedestal.png"),
			Vector3(x, 0, z));

		//s->SetYScale(8);
		//s->SetZScale(2);
		s->SetTag("Level");

		m_gameObjects.push_back(s);
	}

	//Spawn Rubies
	for (int i = 0; i < 10; i++)
	{
		float x = MathsHelper::RandomRange(-20, 20);
		float z = MathsHelper::RandomRange(-20, 20);
		m_player->registerRuby();

		Ruby* r = new Ruby(m_meshManager->GetMesh("Assets/Meshes/ruby.obj"),
			m_diffuseTexturedShader,
			m_textureManager->GetTexture("Assets/Textures/gradient_redPink.png"),
			Vector3(x, 1, z));

		m_gameObjects.push_back(r);
		m_rubies.push_back(r);

		//Pedestal
		StaticObject * s = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ammoBlock.obj"),
			m_diffuseTexturedShader,
			m_textureManager->GetTexture("Assets/Textures/pedestal.png"),
			Vector3(x, 0, z));
		
		//s->SetYScale(8);
		//s->SetZScale(2);
		s->SetTag("Level");
		
		m_gameObjects.push_back(s);

	}
}

void Game::Update(float timestep)
{
	m_input->BeginUpdate();
	RefreshUI();

	if (m_input->GetKeyDown(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	m_simTime = m_player->getSimSpeed();

	//Spawn player bullets
	if(m_input->GetMouseUp(0) && m_player->canFire())
	{
		Bullet* b = m_player->SpawnBullet(
			m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
			m_diffuseTexturedShader,
			m_textureManager->GetTexture("Assets/Textures/bullet.png")
		);

		m_gameObjects.push_back(b);
	}

	//Update objects
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(timestep, m_simTime);	
	}

	//Spawn enemy bullets
	for (unsigned int i = 0; i < m_enemies.size(); i++)
	{
		if(m_enemies[i]->CanShoot())
		{
			m_gameObjects.push_back(m_enemies[i]->SpawnBullet(
				m_meshManager->GetMesh("Assets/Meshes/bullet.obj"),
				m_diffuseTexturedShader,
				m_textureManager->GetTexture("Assets/Textures/bullet.png"))
			);
		}
	}

	//Destroy if marked
	//Wasn't game to put in in the other loop for fear of resizing issues/ missing updates
	for (unsigned int i = 0; i < m_gameObjects.size(); i++) 
	{
		if (m_gameObjects[i]->MarkedForDestroy())
		{
			delete m_gameObjects[i]; //Remove from memory
			m_gameObjects.erase(m_gameObjects.begin() + i); //remove pointer from array and resize
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
	m_arialFont18->DrawString(m_spriteBatch, L"ESC to quit", Vector2(20, 50), Color(1,1,1,0.7f), 0, Vector2(0,0));

	m_arialFont23->DrawString(m_spriteBatch, m_ammoText.c_str(), Vector2(20, 650), Color(1, 1, 1), 0, Vector2(0, 0),Vector2(2,2),SpriteEffects_None,1);

	m_arialFont23->DrawString(m_spriteBatch, m_rubyText.c_str(), Vector2(1100, 650), Color(1, 0.8f, 0.8f), 0, Vector2(0, 0), Vector2(2, 2), SpriteEffects_None, 1);
	
	//Crosshair
	//m_spriteBatch->Draw(m_crossHair->GetShaderResourceView(), Vector2(990/2, 540/2), Color(1.0f, 1.0f, 1.0f));	

	//Health bar
	m_spriteBatch->Draw(m_healthBar->GetShaderResourceView(), Vector2(500, 20), nullptr, Color(1.0f, 1.0f, 1.0f),0,Vector2(0,0),Vector2(m_player->getHealth() * 2, 1), SpriteEffects_None, 1);

	//Hurt Overlay (Top element)
	m_spriteBatch->Draw(m_hurtOverlay->GetShaderResourceView(), Vector2(0, 0), Color(1.0f, 1.0f, 1.0f, m_player->GetHurtAlpha()));
	
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