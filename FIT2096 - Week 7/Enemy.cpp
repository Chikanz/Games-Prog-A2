#include "Enemy.h"
#include <iostream>

Enemy::Enemy(Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
: PhysicsObject(mesh, shader, texture, position) 
{
	updateBounds();

	m_coolDownThresh = MathsHelper::RandomRange(1.5f, 2.0f);

	m_player = player;
	m_tag = "Enemy";
};

void Enemy::Update(float timestep, float simSpeed)
{
	//Face player
	//Adapted from https://gamedev.stackexchange.com/questions/49613/how-to-rotate-enemy-to-face-player
	Vector3 p = m_player->GetPosition();
	float targetrotation = atan2(m_position.z - p.z, m_position.x - p.x);
	m_rotY = -targetrotation + ToRadians(-90); //HAHAHAHAH I DON'T UNDERSTAND MATH

	m_coolDown += timestep * simSpeed;

	updateBounds();
	PhysicsObject::Update(timestep, simSpeed);
}

void Enemy::GetShot()
{
	health -= 1;

	if(health <= 0)
	{
		Destroy();
	}
}

void Enemy::OnCollisionEnter(GameObject* other)
{
	if (other->GetTag() == "Enemy" || other->GetTag() == "Level")
	{
		ApplyForce(-m_velocity * 2);
	}
}

bool Enemy::CanShoot()
{
	if(m_coolDown > m_coolDownThresh)
	{
		m_coolDown = 0;
		return true;
	}
	return false;
}

Bullet* Enemy::SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture) const
{
	Bullet* b = new Bullet(m_tag, mesh, shader, texture, m_barrelPos + m_position);
	b->SetYRotation(m_rotY);
	return b;
}

