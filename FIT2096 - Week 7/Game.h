/*	FIT2096 - Example Code
*	Game.h
*	Created by Elliott Wilson & Mike Yeates - 2016 - Monash University
*	This class is the heart of our game and is also where our game logic will reside
*	It contains the overall Update and Render method for the whole game
*	You should eventually split some game logic into other objects which Game will manage
*	Watch the size of this class - it can grow out of control very fast!
*/

#ifndef GAME_H
#define GAME_H

#include "Direct3D.h"
#include "Camera.h"
#include "InputController.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "GameObject.h"

#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

#include <vector>
#include "player.h"
#include "Enemy.h"

#include "TexturedShader.h"
#include "SceneLighting.h"
#include "TextMan.h"
#include "Trigger.h"

//Forward decleration because C++ is weird
struct EnemyInfo; 

class Game
{
private:
	Camera* m_currentCam;		
	Direct3D* m_renderer;
	InputController* m_input;
	MeshManager* m_meshManager;
	TextureManager* m_textureManager;
	CollisionManager* m_collisionManager;

	// Sprites / Fonts
	SpriteBatch* m_spriteBatch;
	SpriteFont* m_arialFont12;
	SpriteFont* m_arialFont18;
	SpriteFont* m_arialFont23;
	SpriteFont* m_roboto128;

	// Shaders
	Shader* m_unlitVertexColouredShader;
	Shader* m_unlitTexturedShader;
	Shader* m_diffuseTexturedShader;
	Shader* m_diffuseTexturedFogShader;

	float m_simTime = 1; //Slow mo time factor from player
	Player* m_player;

	//Vectors for dayssss!
	vector<GameObject*> m_gameObjects;	
	vector<Enemy*> m_enemies;
	vector<Trigger*> m_triggers;

	vector<CBoundingBox>* LevelBounds;
	vector<EnemyInfo>* Enemies;

	Texture* m_crossHair;
	Texture* m_hurtOverlay;
	Texture* m_healthBar;

	SceneLighting* m_sceneLighting;
	
	// Initialisation Helpers
	bool InitShaders();
	bool LoadMeshes();
	bool LoadTextures();
	void LoadFonts();
	void InitGameWorld();

	void InitLights();

	void InitUI();
	void DrawUI();
	void RefreshUI();

	void AddStartText();
	void InitTriggers();

	void GameOver(bool win);
	bool ended = false;

	void SpawnEnemies();
	void SpawnLevelBounds();

	Enemy* SpawnEnemy(float x, float z, float yRot, Enemy::eAction action, bool gun);
	TextMan* TM;


public:
	Game();	
	~Game();

	bool Initialise(Direct3D* renderer, InputController* input); //The initialise method will load all of the content for the game (meshes, textures, etc.)

	void Update(float timestep);	//The overall Update method for the game. All gameplay logic will be done somewhere within this method
	void Render();					//The overall Render method for the game. Here all of the meshes that need to be drawn will be drawn	

	void Shutdown(); //Cleanup everything we initialised
};

#endif