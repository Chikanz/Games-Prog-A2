#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
private:
	float m_bulletSpeed = 50; 
	CBoundingBox m_bounding;
	string m_owner;

	float bulletAliveTimer = 0;
	float bulletKillTime = 5;

public:
	Bullet(string owner, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float deltaTime, float simTime);
	void OnCollisionEnter(GameObject* other);

};
