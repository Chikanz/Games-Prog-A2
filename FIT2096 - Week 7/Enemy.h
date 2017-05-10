//Abstract enemy class
#pragma once
#include "PhysicsObject.h"
#include "player.h"
#include "MathsHelper.h"

class Enemy : public PhysicsObject
{
public:
	enum eAgentType
	{
		CHASER,		//Chases the player
		WANDERER,	//Wanders around randomly
		LOOTER,		//Moves to random ruby
		COMMANDER,	//Flees when player gets near
	};

protected :
	Vector3 m_target;
	float m_coolDownThresh;
	float m_coolDown;
	float m_moveSpeed = 0.005f;
	Player* m_player;
	int health = 1;
	const bool debugMode = false;
	bool isDead = false;

	bool fallDirection = false;
	bool m_atTarget;

	float m_deathRotX;
	//float timeElapsed;

	float m_deadTimer;
	const float m_deadThresh = 7;

	float timeElapsed;

	Vector3 m_barrelPos = Vector3(-0.133f,1.196f,0.376f);
	eAgentType eType;	

public:
	Enemy(eAgentType type, Player* player, vector<Ruby*>* rubies, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void OnCollisionEnter(GameObject* other);
	bool CanShoot();
	Bullet* SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture) const;
	virtual void Update(float timestep, float simSpeed);
	void GetShot();
	eAgentType GetType() { return eType; };
public:
	
};
