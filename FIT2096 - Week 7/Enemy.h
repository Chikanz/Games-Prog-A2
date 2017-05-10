//Abstract enemy class
#pragma once
#include "PhysicsObject.h"
#include "player.h"
#include "MathsHelper.h"
#include "Gun.h"

class Enemy : public PhysicsObject
{
public:

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

public:
	Enemy(Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void OnCollisionEnter(GameObject* other);
	bool CanShoot();
	Bullet* SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture) const;
	virtual void Update(float timestep, float simSpeed);
	void GetShot();	

	void GrabGun(Gun* g);
public:
	
};
