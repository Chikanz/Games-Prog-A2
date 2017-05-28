#include "Enemy.h"
#include <iostream>

Enemy::Enemy(eAction _action,Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position, AudioSystem* _AS)
: PhysicsObject(mesh, shader, texture, position) 
{
	AS = _AS;

	m_coolDownThresh = MathsHelper::RandomRange(1.5f, 2.0f);
	m_coolDown = MathsHelper::RandomRange(0.0f, m_coolDownThresh * 0.8f);

	m_player = player;
	m_tag = "Enemy";

	m_action = _action;

	//Vary fall direction TODO make int 
	fallDirection = true;
	if(MathsHelper::RandomRange(0,100) > 50)
		fallDirection = false;

	UpdateBounds();
};

void Enemy::UpdateBounds()
{
	//Make hitboxes more unforgiving
	Vector3 min = m_position + m_mesh->GetMin() * 0.3f;
	Vector3 max = m_position + m_mesh->GetMax() * 0.3f;
	min.y = (m_position + m_mesh->GetMin()).y;
	max.y = (m_position + m_mesh->GetMax()).y;

	m_bounds = CBoundingBox(min, max);
}

//probably should have used dot product now that I think about it
//https://gamedev.stackexchange.com/questions/49613/how-to-rotate-enemy-to-face-player
float Enemy::getRotation(float target, float turnSpeed, float timeStep, float simSpeed)
{
	//get difference in degrees between both angles
	float rotationDifference = m_rotY - target;
	if (rotationDifference < 0.01f) return target; //Lock on
	
	//if difference is greater than 180 degrees, reverse rotating direction
	//by adding or subtracting 360 degrees
	if (abs(rotationDifference) > 180) 
	{
		rotationDifference += (rotationDifference > 0 ? -360 : 360);
	}	

	//based on difference, rotate enemy angle
	if (rotationDifference < 0) {
		return m_rotY + (turnSpeed * simSpeed * timeStep);
	}
	else if (rotationDifference > 0) {
		return m_rotY - (turnSpeed * simSpeed * timeStep);
	}
}

bool Enemy::CanSeePlayer(Vector3 enemy2Player)
{	
	if (enemy2Player.Length() < seeDistance && //See distance
		m_facing.Dot(enemy2Player) > 0) //If positive enemy can see player!
	{
		return true;
	}
	return false;
}

//Face player, Adapted from https://gamedev.stackexchange.com/questions/49613/how-to-rotate-enemy-to-face-player			
float Enemy::getTargetRot(Vector3 p)
{
	return -(atan2(m_position.z - p.z, m_position.x - p.x)) + ToRadians(-90); //what is maths even
}

void Enemy::Update(float timestep, float simSpeed)
{
	if (!isDead)
	{
		//Prepare for math magic http://blog.wolfire.com/2009/07/linear-algebra-for-game-developers-part-2/
		Vector3 playerPos = m_player->GetCamPosition();
		Vector3 myPos = m_position;
		playerPos.y = 0;
		myPos.y = 0;
		Vector3 V = playerPos - myPos; //Vector to player

		//Get My local forward
		Vector3 worldForward = Vector3(0, 0, 1);
		Matrix heading = Matrix::CreateRotationY(m_rotY) * Matrix::CreateRotationX(m_rotX);
		m_facing = Vector3::TransformNormal(worldForward, heading); //My facing		


		float targetRotation; //Case statements in C++ are weird

		switch (m_action)
		{
		case IDLE:
			Stop();
			if(CanSeePlayer(V))
				m_action = ATTACKING;
			break;

		case SEARCHING:
			
			if (atTarget) //Set target when reached
			{				
				m_velocity = Vector3::Zero;
				m_target = Vector3(MathsHelper::RandomRange(-5, 5) + m_position.x, 0, MathsHelper::RandomRange(-5, 5) + m_position.z);				
				atTarget = false;

				if (moving) //Repositioning 
				{
					m_action = ATTACKING;
					fireCount = 0;
					m_coolDown = m_coolDownThresh - 0.5f;
				}
			}
			else
				atTarget = Vector3::Distance(m_position, m_target) < 0.1f;

			//If see the player while searching and not repositioning
			if (CanSeePlayer(V) && !moving && m_Gun)
			{
				m_action = ATTACKING;
				fireCount = 0;
				m_coolDown = m_coolDownThresh - 0.5f;
			}

			targetRotation = getTargetRot(m_target);
			m_rotY = getRotation(targetRotation, 5, timestep, simSpeed);

			break;

		case ATTACKING:

			if (!m_Gun) //If we see the player but have no gun run around helplessly
			{
				atTarget = true;
				m_action = SEARCHING;
			}

			Stop();
			targetRotation = getTargetRot(m_player->GetCamPosition());
			m_rotY = getRotation(targetRotation, 5, timestep, simSpeed);
			m_coolDown += timestep * simSpeed;

			//Enemy reposition
			if (fireCount > 3)
			{
				atTarget = false;
				m_target = Vector3(MathsHelper::RandomRange(-5, 5) + m_position.x, 0, MathsHelper::RandomRange(-5, 5) + m_position.z);
				m_action = SEARCHING;
				moving = true;
				fireCount = 0;
			}


			//If player runs
			//tempted to do some sort of cool raycasting thing to get line of sight...
			if(V.Length() > seeDistance)
			{
				atTarget = false;
				moving = false;
				m_action = SEARCHING;
			}
			break;
		}

		//Common
		if(m_action == SEARCHING)
			ApplyForce((m_target - m_position) * (m_moveSpeed * simSpeed));
	}
	else //Ded
	{
		m_velocity = Vector3::Zero;
		m_acceleration = Vector3::Zero;

		//Sink
		ApplyForce(Vector3(0, -1.5f, 0) * 50 * timestep * simSpeed);

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

		AudioClip* clip = AS->Play("Assets/Sounds/SplatSplat.wav", false);
		clip->SetVolume(1);
		clip->SetIs3D(true);
		clip->Set3DAttributes(m_position, m_velocity);
	}
}


void Enemy::Melee()
{
}

void Enemy::GrabGun(Gun* g)
{
	if(g->SetOwner(this))
		m_Gun = g;
}

void Enemy::Dummy()
{
	Stop();	
	m_rotY = getTargetRot(m_player->GetCamPosition());
}

void Enemy::OnCollisionEnter(GameObject* other)
{
	if (isDead) return;
	if (other->GetTag() == "Enemy" || other->GetTag() == "Level" || other->GetTag() == "Player")
	{
		ApplyForce(-m_velocity * 1);
	}

	if(other->GetTag() == "Level")
	{
		if (m_action == SEARCHING)
		{
			atTarget = true;		
		}
	}

}

bool Enemy::CanShoot()
{
	if(m_coolDown > m_coolDownThresh &&
		!isDead && m_Gun && m_action == ATTACKING)
	{
		m_coolDown = 0;
		return true;
	}
	return false;
}

Bullet* Enemy::SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture)
{
	fireCount += 1;
	Bullet* b = new Bullet(m_tag, mesh, shader, texture, m_Gun->GetPosition() + m_position);
	b->SetYRotation(m_rotY);

	AudioClip* clip = AS->Play("Assets/Sounds/shotshot.wav", false);
	clip->SetVolume(0.8f);
	clip->SetMinMaxDistance(30.0f, 200.0f);
	clip->SetIs3D(true);
	clip->Set3DAttributes(m_position, Vector3::Zero);
	return b;
}



