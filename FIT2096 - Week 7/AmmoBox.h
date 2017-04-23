#pragma once

#include"GameObject.h"

class AmmoBox : public GameObject
{
private:
	CBoundingBox bounds;
	int bullets = 9;

public:
	int getBullets() { return bullets; };
	CBoundingBox GetBounds() { return bounds; };
	AmmoBox(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float timeStep, float simTime);

	//void OnCollisionEnter(GameObject* other);
	
};
