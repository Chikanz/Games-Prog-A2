#pragma once
#include "FlyingCamera.h"

class Player : public FlyingCamera
{
private:	
	int m_health = 5;
	int m_inClip = 9; //Bullets currently in gun
	int m_ammo = 27;	//Bullets on person (3 clips)
	int m_clipCap = m_inClip; //Default clip size
	float m_coolDown = 99.0f; 
	float m_fireTime = 0.2f;

	CBoundingBox m_bounds;
	void UpdateBounds();
	Mesh* m_colliderMesh;
	float m_height;

	string m_tag = "Player";

	//Rupies
	int rubiesHeld = 0;
	int maxRubies = 0;

	//hurt overlay
	float hurtDuration = 1;
	float hurtTimer = hurtDuration + 1;

public:
	void Update(float timeStep);
	Player(InputController* input, Vector3 startPos, Mesh* colliderMesh);

	//Bullet spawning 
	Bullet* SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture);
	bool canFire();

	//Getters
	float getSimSpeed() { return m_simSpeed; }
	int getInClip() { return m_inClip; }
	int getAmmo() { return m_ammo; }
	int getHealth() { return m_health; };

	int GetRubiesHeld() { return rubiesHeld; };
	int GetMaxRubies() { return maxRubies; };
	void registerRuby() { maxRubies++; };

	//Collisions
	CBoundingBox GetBounds() { return m_bounds; };
	
	//Collision
	void OnCollisionStay(GameObject* other);
	void OnCollisionEnter(GameObject* other);
	void OnCollisionExit(GameObject* other);

	//Hurt overlay timer 
	float GetHurtAlpha();

	
};
