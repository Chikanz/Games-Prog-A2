#pragma once
#include "PhysicsObject.h"
#include "InputController.h"

class Gun : public PhysicsObject
{
	
private:
	GameObject* m_owner = nullptr;
	int m_ammo = 9;
	float coolDown = -1;
	const float coolDownReset = 0.3f;

public:
	Gun(InputController* input, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void SetOwner(GameObject* newOwner);
	void RemoveOwner(Vector3 worldPos);
	void Update(float timeStep, float simSpeed);
	void Render(Direct3D* renderer, Camera* cam);

	Vector3 enemyGrabPos = Vector3(-0.15f,1.2f,0.4f);
	Vector3 playerGrabPos = Vector3(0.4,-0.2,0.7);

	Vector3 grabPos;
	InputController* m_input;
	bool playerOwned = true;

	GameObject* GetOwner() { return m_owner; };

	int GetAmmo() { return m_ammo; }
	bool CanFire();
	void Fire(float force);

	void OnCollisionEnter(GameObject* other);

};
