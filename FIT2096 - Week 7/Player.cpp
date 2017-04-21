#include "player.h"
#include <iostream>
#include "MathsHelper.h"

Player::Player(InputController* input, Vector3 startPos) 
: FlyingCamera(input, startPos)
{

}

Bullet* Player::SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture)
{
	Bullet* b = new Bullet(mesh, shader, texture, m_lookAtTarget);
	b->SetXRotation(m_pitch);
	b->SetYRotation(m_heading);
	return b;
}

bool Player::canFire()
{
	if (m_coolDown >= m_fireTime && m_inClip > 0)
	{
		m_coolDown = 0;
		m_inClip -= 1;
		return true;
	}

	return false;
}


void Player::Update(float timeStep)
{
	//Reload
	if (m_input->GetKeyDown('R'))
	{
		if (m_inClip < m_clipCap && m_ammo > 0) 
		{
			int ammoToTake = m_clipCap - m_inClip;
			m_inClip += ammoToTake;
			m_ammo -= ammoToTake;

			m_ammo = MathsHelper::Clamp(m_ammo, 0, 999); //Make sure nothing cheeky happens
		}
	}

	//Update camera logic first
	FlyingCamera::Update(timeStep);

	//Shooty cooldown
	m_coolDown += timeStep * getSimSpeed(); //Update cooldown in relation to simTime
	std::cout << m_coolDown << std::endl;

}

