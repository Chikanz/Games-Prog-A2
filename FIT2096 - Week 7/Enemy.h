//Abstract enemy class
#pragma once
#include "PhysicsObject.h"
#include "player.h"
#include "MathsHelper.h"
#include "Gun.h"

class Enemy : public PhysicsObject
{
public:
	enum eAction
	{
		IDLE,		//Standing still
		SEARCHING,	//Searching for player, Also covers repositioning since the code is similar
		ATTACKING,	//Shooting player	
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

	Gun* m_Gun;	

	Vector3 m_facing;

	eAction m_action = IDLE;
	bool fallDir = false; //false = backwards, true = forwards
	int fireCount = 0;
	const int seeDistance = 15;
	bool moving = false; //Is repositioning or searching
	bool atTarget = true;

	AudioSystem* AS;

	void UpdateBounds();
	float getRotation(float target, float turnSpeed, float timeStep, float simSpeed);
	bool CanSeePlayer(Vector3 V);
	float getTargetRot(Vector3 p);

public:
	Enemy(eAction _action, Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, AudioSystem* _AS);
	void OnCollisionEnter(GameObject* other);
	bool CanShoot();
	Bullet* SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture);
	virtual void Update(float timestep, float simSpeed);
	void GetShot();	
	bool IsDead() { return isDead; };
	void Melee();
	void GrabGun(Gun* g);
	void Dummy();
public:
	
};
