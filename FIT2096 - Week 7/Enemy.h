//Abstract enemy class
#pragma once
#include "PhysicsObject.h"
#include "player.h"
#include "MathsHelper.h"

class Enemy : public PhysicsObject
{
protected :
	Vector3 m_target;
	float m_coolDownThresh;
	float m_coolDown;
	float m_moveSpeed = 0.005f;
	Player* m_player;
	int health = 1;

	const bool debugMode = false;

	Vector3 m_barrelPos = Vector3(-0.133f,1.196f,0.376f);
	Enemy(Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);

public:
	void OnCollisionEnter(GameObject* other);
	bool CanShoot();
	Bullet* SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture) const;
	virtual void Update(float timestep, float simSpeed);
	void GetShot();

public:
	
};
