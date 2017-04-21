#include "bullet.h"


Bullet::Bullet(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
GameObject(mesh, shader, texture, position)
{
	m_bounding = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());	
}

void Bullet::Update(float timeStep, float simTime)
{
	//Past attempts
	//m_position += m_heading * m_bulletSpeed * timeStep * simTime;
	//m_position.z += m_bulletSpeed * timeStep * simTime;
	
	//Get Local forward
	Vector3 worldForward = Vector3(0, 0, 1);
	Matrix heading = Matrix::CreateRotationY(m_rotY);
	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);

	//Make bullet go forward locally 
	m_position += localForward * m_bulletSpeed * timeStep * simTime;
}

