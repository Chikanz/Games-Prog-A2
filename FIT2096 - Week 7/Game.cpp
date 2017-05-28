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
#include <iostream>
#include "StaticBounds.h"
#include <ctime>
#include "Window.h"
#include "FileReader.h"


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
	TM = NULL;
}

Game::~Game() {}

bool Game::Initialise(Direct3D* renderer, InputController* input, AudioSystem* as)
{
	m_AudioSystem = as;
	m_renderer = renderer;	
	m_input = input;
	m_meshManager = new MeshManager();
	m_textureManager = new TextureManager();

	InitLights();

	if (!m_AudioSystem->Initialise())
		return false;

	if (!InitShaders())
		return false;

	if (!LoadMeshes())
		return false;

	if (!LoadTextures())
		return false;

	if (!LoadSounds())
		return false;

	
	LoadFonts();
	InitUI();
	InitGameWorld();	

	AddStartText();
	InitTriggers();
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

	m_diffuseTexturedShader = new TexturedShader(m_sceneLighting);
	if (!m_diffuseTexturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/DiffuseTexturedPixelShader.ps"))
		return false;

	m_diffuseTexturedFogShader = new TexturedShader(m_sceneLighting);
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

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/enemy2.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ground.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/ruby.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/gun.obj"))
		return false;

	if (!m_meshManager->Load(m_renderer, "Assets/Meshes/level.obj"))
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

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/wall2.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/orange.png"))
		return false;

	if (!m_textureManager->Load(m_renderer, "Assets/Textures/red.png"))
		return false;

	return true;
}

bool Game::LoadSounds()
{
	if (!m_AudioSystem->Load("Assets/Sounds/ambient.wav"))		return false;
	if (!m_AudioSystem->Load("Assets/Sounds/gunPickup.wav"))	return false;
	if (!m_AudioSystem->Load("Assets/Sounds/gunThrowaway.wav")) return false;
	if (!m_AudioSystem->Load("Assets/Sounds/neckcrack1.wav"))	return false;
	if (!m_AudioSystem->Load("Assets/Sounds/shotshot.wav"))		return false;
	if (!m_AudioSystem->Load("Assets/Sounds/SplatSplat.wav"))	return false;
	if (!m_AudioSystem->Load("Assets/Sounds/migawka.wav"))		return false; //turns out migawka is actually polish for shutter. TIL.
	if (!m_AudioSystem->Load("Assets/Sounds/shotenemy2.wav"))	return false;
	if (!m_AudioSystem->Load("Assets/Sounds/emptyammo.wav"))	return false;
	if (!m_AudioSystem->Load("Assets/Sounds/super2.wav"))	return false;
	if (!m_AudioSystem->Load("Assets/Sounds/hot2.wav"))	return false;

	return true;
}

void Game::LoadFonts()
{	
	m_roboto128 = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Roboto-128pt.spritefont");
}

void Game::InitUI()
{
	TM = TextMan::GetTextMan(m_AudioSystem);
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext());
	m_crossHair = m_textureManager->GetTexture("Assets/Textures/crosshair.png");
	m_hurtOverlay = m_textureManager->GetTexture("Assets/Textures/sprite_hurtOverlay.png");
	m_healthBar = m_textureManager->GetTexture("Assets/Textures/sprite_healthBar.png");
	m_redOverlay = m_textureManager->GetTexture("Assets/Textures/red.png");

	// Also init any buttons here
}
void Game::AddStartText()
{
	TM->AddText("IT'S"	, 0.8f);
	TM->AddText("ABOUT"	, 0.8f);
	TM->AddText("TIME"	, 1.5f);
}

void Game::InitTriggers()
{
	//Positions stolen from maya, would read them from file if I had time
	m_triggers.push_back(new Trigger(m_player, Vector3(-1.6, 0, 7), 0.5f, "TIME ONLY MOVES WHEN YOU MOVE", TM));
	m_triggers.push_back(new Trigger(m_player, Vector3(6.4, 0, 31), 0.5f, "TAKE HIM DOWN", TM));
	m_triggers.push_back(new Trigger(m_player, Vector3(-4, 0, 31), 0.5f,  "TAKE HIS GUN", TM));
	m_triggers.push_back(new Trigger(m_player, Vector3(-18, 0, 31), 0.5f, "KILL THEM ALL", TM));
}

void Game::GameOver(bool win)
{
	if (win)
	{
		if (ended) return;
		ended = true;

		for (int i = 0; i < 50; i++)
		{
			TM->AddText("SUPER", 0.6f);
			TM->AddText("HOT", 0.6f);
		}
		m_player->ThrowGun();
		m_player->Freeze(true);
		m_player->TriggerOverlay();
	}
	else //Restartarino
	{
		//Text
		TM->Clear();
		AddStartText();
		InitTriggers();

		//Player
		m_player->Reset();
		Enemies->size();

		//Enemies + guns
		//Delet
		for (unsigned int i = 0; i < m_gameObjects.size(); i++)
		{
			if(m_gameObjects[i]->GetTag() == "Enemy" || 
				m_gameObjects[i]->GetTag() == "Gun")
			{
				m_gameObjects[i]->Destroy();
			}
		}
		m_enemies.clear();
		SpawnEnemies();
	}
}

void Game::SpawnEnemies()
{
	for (int i = 0; i < Enemies->size(); i++)
	{
		EnemyInfo e = (*Enemies)[i];

		if (i == 3) //The intro dummy enemy, like in the super hot demo
		{
			Enemy* enemy = SpawnEnemy(e.Xpos, e.Zpos, 0, Enemy::eAction::ATTACKING, false);
			enemy->Dummy();
			enemy->GetShot();
		}
		else //Normal spawning
		{
			SpawnEnemy(e.Xpos, e.Zpos, e.YRot, e.action, true);
		}
	}
}

Enemy* Game::SpawnEnemy(float x, float z, float yRot, Enemy::eAction action, bool gun)
{
	//Make enemy
	Enemy* e = new Enemy(action,
		m_player,		
		m_meshManager->GetMesh("Assets/Meshes/enemy2.obj"),
		m_diffuseTexturedShader,
		m_textureManager->GetTexture("Assets/Textures/gradient_red.png"),
		Vector3(x, 0, z),
		m_AudioSystem);

	e->SetYRotation(ToRadians(yRot));

	if (gun)
	{
		Gun* g = new Gun(m_input,
			m_meshManager->GetMesh("Assets/Meshes/gun.obj"),
			m_diffuseTexturedFogShader,
			m_textureManager->GetTexture("Assets/Textures/orange.png"),
			Vector3(-999,-999,-999));

		e->GrabGun(g);
		m_gameObjects.push_back(g);
	}

	m_gameObjects.push_back(e);
	m_enemies.push_back(e);
	return e;
}

void Game::InitGameWorld()
{
	//Init Collision manager
	m_collisionManager = new CollisionManager(&m_gameObjects);

	//Multiple inheritance Player	
	m_player = new Player(
		m_input, Vector3(0, 1.5f, -1), m_meshManager->GetMesh("Assets/Meshes/enemy2.obj"),m_collisionManager, m_AudioSystem);
	m_currentCam = m_player;
	m_gameObjects.push_back(m_player);

	//Init Collision manager
	m_collisionManager = new CollisionManager(&m_gameObjects);

	//Get level bounds from file
	FileReader f = FileReader();
	LevelBounds = f.ReadBounds();
	for (int i = 0; i < LevelBounds->size(); i++)
	{
		StaticBounds* s = new StaticBounds((*LevelBounds)[i]);
		s->SetTag("Level");
		m_gameObjects.push_back(s);		
	}

	//Get enemy info from file 
	Enemies = f.ReadEnemies();
	SpawnEnemies();

	//Ground
	m_gameObjects.push_back(new StaticObject(m_meshManager->GetMesh("Assets/Meshes/ground.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/ground.png")));

	//Level
	GameObject* level = new StaticObject(m_meshManager->GetMesh("Assets/Meshes/level.obj"),
		m_diffuseTexturedFogShader,
		m_textureManager->GetTexture("Assets/Textures/pedestal.png"));
	m_gameObjects.push_back(level);	

	AudioClip* c = m_AudioSystem->Play("Assets/Sounds/ambient.wav", false);
	if (c)
	{
		c->SetLoopCount(-1);
		c->SetIs3D(false);
		c->SetVolume(1);
		c->TimeShift = false;
	}

}

void Game::InitLights()
{
	m_sceneLighting = new SceneLighting
	(
		Vector3(1, 1, 1), // Light direction
		Color(1, 1, 1, 1.0f) * 0.8f, // Light colour
		Color(0.4f, 0.4f, 0.4f, 1.0f), // Ambient colour
		Color(0.1f, 0.1f, 0.1f, 1.0f), // Fog colour
		5.0f, // Fog start distance
		100.0f  // Fog end distance
	);
}

void Game::Update(float timestep)
{
	m_input->BeginUpdate();

	Enemies->size();

	//Check win condition
	for(int i = 0; i < m_enemies.size(); i++)
	{
		if (!m_enemies[i]->IsDead()) break;

		if(i == m_enemies.size() - 1)
		{			
			GameOver(true);
		}
	}
	//Check lose condition
	if(m_player->isDead()) GameOver(false);

	if (m_input->GetKeyDown(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	m_simTime = m_player->getSimSpeed();

	//Update UI
	TM->Update(timestep);

	//Update Triggers
	for(int i = 0; i < m_triggers.size(); i++)
	{
		m_triggers[i]->Update();
	}

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

	//Testing
	//if (m_input->GetKeyDown('R'))
	//{
	//	if (!m_player->m_gun)
	//	{
	//		Gun* g = new Gun(m_input,
	//			m_meshManager->GetMesh("Assets/Meshes/gun.obj"),
	//			m_diffuseTexturedFogShader,
	//			m_textureManager->GetTexture("Assets/Textures/pedestal.png"),
	//			Vector3::Zero);
	//		m_gameObjects.push_back(g);
	//		m_player->GrabGun(g);			
	//	}
	//}

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

	//Destroy if marked ... didn't have time for memory pools :(
	//Wasn't game to put in in the other loop for fear of resizing issues/ missing updates
	for (unsigned int i = 0; i < m_gameObjects.size(); i++) 
	{
		if (m_gameObjects[i]->MarkedForDestroy())
		{
			delete m_gameObjects[i]; //Remove from memory
			m_gameObjects[i] = nullptr;
			m_gameObjects.erase(m_gameObjects.begin() + i); //remove pointer from array and resize
		}
	}
	
	m_AudioSystem->Update(m_player->getSimSpeed());
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
	
	Vector2 windowSize = Window::g_window->GetDimens();

	//Crosshair
	m_spriteBatch->Draw(m_crossHair->GetShaderResourceView(), Vector2(0, 0), nullptr,
		Color(1.0f, 1.0f, 1.0f),0,Vector2(0,0),Vector2(1, 1.1f), SpriteEffects_None, 1);
	
	

	//Reddd or overlay
	if (ended) 
		m_spriteBatch->Draw(m_redOverlay->GetShaderResourceView(), Vector2(0, 0), nullptr, Color(1.0f, 1.0f, 1.0f, 1 - m_player->GetHurtAlpha()),0,Vector2(0,0),Vector2(1000, 10000), SpriteEffects_None, 1);
	else
		m_spriteBatch->Draw(m_hurtOverlay->GetShaderResourceView(), Vector2(0, 0), Color(1.0f, 1.0f, 1.0f, m_player->GetHurtAlpha()));
	
	//Main text
	Vector2 origin = m_roboto128->MeasureString(TM->getText().c_str()) / 2.f; //Center text https://github.com/Microsoft/DirectXTK/wiki/Drawing-text
	m_roboto128->DrawString(m_spriteBatch, TM->getText().c_str(), Vector2(windowSize.x / 2, windowSize.y / 2), Color(1, 1, 1), 0, origin, Vector2(1, 1), SpriteEffects_None, 1);
	
	m_spriteBatch->End();
}


void Game::Shutdown()
{
	m_AudioSystem->Shutdown();

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		delete m_gameObjects[i];
	}

	m_gameObjects.empty();

	if (m_currentCam)
	{
		//delete m_currentCam;
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