#pragma once
#include "FlyingCamera.h"

class Player : public FlyingCamera
{
private:	
	int m_health;
	int m_inClip = 9; //Bullets currently in gun
	int m_ammo = 27;	//Bullets on person (3 clips)
	int m_clipCap = m_inClip; //Default clip size
	float m_coolDown = 99.0f; 
	float m_fireTime = 0.01f;

public:
	void Update(float timeStep);
	Player(InputController* input, Vector3 startPos);

	Bullet* SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture);
	bool canFire();

	float getSimSpeed() { return m_simSpeed; };
	int getInClip() { return m_inClip; };
	int getAmmo() { return m_ammo; };
};
