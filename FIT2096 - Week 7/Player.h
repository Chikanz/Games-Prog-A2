#pragma once
#include "FlyingCamera.h"
#include "Ruby.h"
#include <vector>
#include "Gun.h"

class Player : public FlyingCamera, public GameObject
{
private:	
	int m_health = 7;
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

	//hurt overlay
	float hurtDuration = 1;
	float hurtTimer = hurtDuration + 1;

	Gun* m_gun;

public:
	void Update(float timeStep);
	void Update(float timestep, float simSpeed);
	void Render(Direct3D* renderer, Camera* cam);

	Player(InputController* input, Vector3 startPos, Mesh* colliderMesh);

	//Bullet spawning 
	Bullet* SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture);
	bool canFire();

	//Getters
	float getSimSpeed() { return m_simSpeed; }
	int getInClip() { return m_inClip; }
	int getAmmo() { return m_ammo; }
	int getHealth() { return m_health; };

	//Collisions
	CBoundingBox GetBounds() { return m_bounds; };
	
	//Collision
	void OnCollisionStay(GameObject* other);
	void OnCollisionEnter(GameObject* other);
	void OnCollisionExit(GameObject* other);

	void GrabGun(Gun* g);

	//Hurt overlay timer 
	float GetHurtAlpha();

	
};
