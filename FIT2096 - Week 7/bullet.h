#pragma once

#include "GameObject.h"

class Bullet : public GameObject
{
private:
	float m_bulletSpeed = 50; 
	CBoundingBox m_bounding;

public:
	Bullet(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float deltaTime, float simTime);
	CBoundingBox GetBounds() { return m_bounding; }
};
