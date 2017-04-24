#pragma once
#include "GameObject.h"

class Ruby : public GameObject
{
private:
	float m_spinSpeed = 5;	

public:
	Ruby(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float timeStep, float simSpeed);
	//void OnCollisionEnter(GameObject* other);
};
