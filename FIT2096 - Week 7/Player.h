#pragma once
#include "FlyingCamera.h"
#include "Ruby.h"
#include <vector>
#include "Gun.h"
#include "CollisionManager.h"

class Player : public FlyingCamera, public GameObject
{
private:	
	int m_health = 7;
	float m_coolDown = 99.0f; 
	float m_fireTime = 0.3f;

	void UpdateBounds();
	Mesh* m_colliderMesh;
	float m_height;

	string m_tag = "Player";	

	//hurt overlay
	float hurtDuration = 1;
	float hurtTimer = hurtDuration + 1;

	CBoundingBox outerBounds;

	//Debug
	Vector3 Editor(Vector3 input);

	//Force sim speed
	float forcedSimSpeed;
	float forceTimer;
	bool forcingSpeed = false;

	CollisionManager* cm;
	vector<GameObject*>* TriggerList = new vector<GameObject*>;

public:
	//TODO REMOVE
	Gun* m_gun;	

	void Update(float timeStep);
	void Update(float timestep, float simSpeed);
	void Render(Direct3D* renderer, Camera* cam);

	Player(InputController* input, Vector3 startPos, Mesh* colliderMesh, CollisionManager* col);

	//Bullet spawning 
	Bullet* SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture);
	bool canFire();

	//Getters
	float getSimSpeed() { return m_simSpeed; };	
	int getHealth() { return m_health; };
	
	//Collision
	void OnCollisionStay(GameObject* other);
	void OnCollisionEnter(GameObject* other);
	void OnCollisionExit(GameObject* other);

	void GrabGun(Gun* g);

	void ForceSimSpeed(float speed, float duration);

	Vector3 GetLookAt() { return m_lookAtTarget; };

	//Hurt overlay timer 
	float GetHurtAlpha();
};
