#pragma once

#include "GameObject.h"

class bullet : public GameObject
{
private:
	float m_bulletSpeed = 50; 
	Vector3 m_heading;
	CBoundingBox m_bounding;

public:
	bullet(Vector3 heading, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float deltaTime, float simTime);
	CBoundingBox GetBounds() { return m_bounding; }
};
