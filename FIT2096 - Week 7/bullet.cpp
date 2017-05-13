#include "bullet.h"


Bullet::Bullet(string owner, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
GameObject(mesh, shader, texture, position)
{
	m_tag = "Bullet";
	m_owner = owner;
	UpdateBounds();
}

void Bullet::Update(float timeStep, float simTime)
{
	//Past attempts
	//m_position += m_heading * m_bulletSpeed * timeStep * simTime;
	//m_position.z += m_bulletSpeed * timeStep * simTime;
	
	bulletAliveTimer += timeStep * simTime;
	if (bulletAliveTimer > bulletKillTime)
	{
		Destroy();
		return;
	}

	//Get Local forward
	Vector3 worldForward = Vector3(0, 0, 1);
	Matrix heading = Matrix::CreateRotationY(m_rotY) * Matrix::CreateRotationX(m_rotX);
	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);

	//Make bullet go forward locally 
	m_position += localForward * m_bulletSpeed * timeStep * simTime;
	UpdateBounds();
}

void Bullet::OnCollisionEnter(GameObject* other)
{	
	if (other->GetTag() != m_owner && //Make sure not shooting self or other bullets
		other->GetTag() != "Bullet")
	{
		other->GetShot(); //Bullet calls getShot so each object doens't have to impliment bullet collision, except player
		Destroy();
	}
}

