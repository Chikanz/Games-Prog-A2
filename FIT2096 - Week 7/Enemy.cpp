#include "Enemy.h"
#include <iostream>

Enemy::Enemy(Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
: PhysicsObject(mesh, shader, texture, position) 
{
	m_coolDownThresh = MathsHelper::RandomRange(1.5f, 2.0f);
	m_coolDown = MathsHelper::RandomRange(0.0f, m_coolDownThresh * 0.8f);

	m_player = player;
	m_tag = "Enemy";

	//Vary fall direction
	fallDirection = true;
	if(MathsHelper::RandomRange(0,100) > 50)
		fallDirection = false;

	UpdateBounds();
};

void Enemy::Update(float timestep, float simSpeed)
{
	m_target = m_player->GetCamPosition();
	m_target.y = 0;

	//Move
	ApplyForce((m_target - m_position) * (m_moveSpeed * simSpeed));

	//Normal update logic
	if (!isDead)
	{
		//Face player
		//Adapted from https://gamedev.stackexchange.com/questions/49613/how-to-rotate-enemy-to-face-player
		Vector3 p = m_player->GetCamPosition();
		float targetrotation = atan2(m_position.z - p.z, m_position.x - p.x);
		m_rotY = -targetrotation + ToRadians(-90); //HAHAHAHAH I DON'T UNDERSTAND MATH

		//Add some innaccuracy
		timeElapsed += timestep;
		m_rotY += sin(timeElapsed) * 5 * simSpeed * timestep;
	}
	else //Ded
	{
		m_velocity = Vector3::Zero;
		m_acceleration = Vector3::Zero;

		ApplyForce(Vector3(0, -1, 0) * 50 * timestep * simSpeed);

		//Move faster on first hit to give the illusion of a heavy impact, even in slowmo
		float simCopy = 1.0f;
		if (abs(m_rotX - m_deathRotX) > 0.3f) simCopy = simSpeed;

		if(fallDirection) 
			m_rotY += timestep * simCopy;
		else
			m_rotY -= timestep * simCopy;
		
		m_rotX -= 2 * timestep * simCopy * 0.8f; //Fall backwards

		//dead timer
		m_deadTimer += timestep * simSpeed;
		if (m_deadTimer >= m_deadThresh)
			Destroy();
	}

	m_coolDown += timestep * simSpeed;
	UpdateBounds();
	PhysicsObject::Update(timestep, simSpeed);
}

void Enemy::GetShot()
{
	health -= 1;

	if(health <= 0 && !isDead) //Trigger die
	{
		if(m_Gun)
			m_Gun->RemoveOwner(m_position);
		isDead = true;
		m_deathRotX = m_rotX;
	}
}

void Enemy::GrabGun(Gun* g)
{
	m_Gun = g;
	g->SetOwner(this);
}

void Enemy::OnCollisionEnter(GameObject* other)
{
	if (isDead) return;
	if (other->GetTag() == "Enemy" || other->GetTag() == "Level" || other->GetTag() == "Player")
	{
		ApplyForce(-m_velocity * 2);
	}
}

bool Enemy::CanShoot()
{
	if(m_coolDown > m_coolDownThresh && !isDead && m_Gun)
	{
		m_coolDown = 0;
		return true;
	}
	return false;
}

Bullet* Enemy::SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture) const
{
	Bullet* b = new Bullet(m_tag, mesh, shader, texture, m_Gun->GetPosition() + m_position);
	b->SetYRotation(m_rotY);
	return b;
}

