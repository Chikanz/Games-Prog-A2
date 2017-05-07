#include "Enemy.h"
#include <iostream>

Enemy::Enemy(eAgentType type, Player* player, vector<Ruby*>* rubies, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
: PhysicsObject(mesh, shader, texture, position) 
{
	m_coolDownThresh = MathsHelper::RandomRange(1.5f, 2.0f);
	m_coolDown = MathsHelper::RandomRange(0.0f, m_coolDownThresh * 0.8f);

	//init types
	eType = type;
	switch(eType)
	{
		case CHASER:
			m_moveSpeed = 0.07f;
			m_frictionAmount = 0.5f;
			break;

		case WANDERER:
			m_moveSpeed = 0.1f;
			m_frictionAmount = 0.5f;
			break;

		case LOOTER:
			m_rubies = rubies;
			m_moveSpeed = 0.1f;
			m_frictionAmount = 0.5f;
			break;

		case COMMANDER:
			m_moveSpeed = 0.1f;
			m_frictionAmount = 0.5f;
			break;

		default:
			assert(false);
	}

	m_player = player;
	m_tag = "Enemy";

	//Vary fall direction
	fallDirection = true;
	if(MathsHelper::RandomRange(0,100) > 50)
		fallDirection = false;

	updateBounds();
};

void Enemy::Update(float timestep, float simSpeed)
{
	//Individual update logic
	switch (eType)
	{
		case CHASER:
			m_target = m_player->GetPosition();
			m_target.y = 0;
			break;

		case WANDERER:
			m_atTarget = Vector3::Distance(m_position, m_target) < 0.1f;
			if (m_atTarget) //Set target when reached
			{
				m_velocity = Vector3::Zero;
				//Traverse using current position, then clamp
				m_target = Vector3(MathsHelper::RandomRange(-5, 5) + m_position.x, 0, MathsHelper::RandomRange(-5, 5) + m_position.z);
				m_target.Clamp(Vector3(-20, -20, -20), Vector3(20, 20, 20));
			}
			break;

		case LOOTER:
			if ((*m_rubies)[rubyIndex] == nullptr)
				m_atTarget = true;

			if (m_atTarget)
			{
				m_atTarget = false;
				
				do { //Make sure position isn't null
					rubyIndex = MathsHelper::RandomRange(0, m_rubies->size() - 1);
				} while ((*m_rubies)[rubyIndex] == nullptr);

				m_target = (*m_rubies)[rubyIndex]->GetPosition(); //set target position 
				m_target.y = 0;
			}
			break;

		case COMMANDER:

			//if standing still
			if (!m_atTarget && Vector3::Distance(m_position, m_target) < 1)
			{
				m_velocity = Vector3::Zero;
				m_atTarget = true;
			}

			//If player gets close and am standing still
			if(m_atTarget && Vector3::Distance(m_position,m_player->GetPosition()) < 3)
			{
				m_atTarget = false;
				m_target = Vector3(MathsHelper::RandomRange(-10, 10) + m_position.x, 0, MathsHelper::RandomRange(-10, 10) + m_position.z);
			}			
			break;

		default:
			assert(false);
	}

	//Move
	ApplyForce((m_target - m_position) * (m_moveSpeed * simSpeed));

	//Normal update logic
	if (!isDead)
	{
		//Face player
		//Adapted from https://gamedev.stackexchange.com/questions/49613/how-to-rotate-enemy-to-face-player
		Vector3 p = m_player->GetPosition();
		float targetrotation = atan2(m_position.z - p.z, m_position.x - p.x);
		m_rotY = -targetrotation + ToRadians(-90); //HAHAHAHAH I DON'T UNDERSTAND MATH

		//Add some innaccuracy (save for proj 2)
		//timeElapsed += timestep;
		//m_rotY += sin(timeElapsed) * 5 * simSpeed * timestep;
	}
	else
	{
		m_velocity = Vector3::Zero;
		m_acceleration = Vector3::Zero;

		ApplyForce(Vector3(0, -1, 0) * 50 * timestep * simSpeed);

		//Move faster on first hit to give the illusion of a heavy impact, even in slowmo
		float simCopy = 1.5f;
		if (abs(m_rotX - m_deathRotX) > 0.5f) simCopy = simSpeed;

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
	updateBounds();
	PhysicsObject::Update(timestep, simSpeed);
}

void Enemy::GetShot()
{
	health -= 1;

	if(health <= 0)
	{
		isDead = true;
		m_deathRotX = m_rotX;
		//Destroy();
	}
}

void Enemy::OnCollisionEnter(GameObject* other)
{
	if (isDead) return;
	if (other->GetTag() == "Enemy" || other->GetTag() == "Level")
	{
		ApplyForce(-m_velocity * 2);
	}
}

bool Enemy::CanShoot()
{
	if(m_coolDown > m_coolDownThresh && !isDead)
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

