#include "AmmoBox.h"

AmmoBox::AmmoBox(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) : 
GameObject(mesh, shader, texture, position)
{
	tag = "ammo";
	updateBounds();
}

void AmmoBox::Update(float timeStep, float simTime)
{
	m_rotY += 10 * timeStep * simTime;
	updateBounds();
}
