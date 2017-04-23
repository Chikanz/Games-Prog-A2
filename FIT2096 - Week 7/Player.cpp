#include "player.h"
#include <iostream>
#include "MathsHelper.h"
#include "AmmoBox.h"

Player::Player(InputController* input, Vector3 startPos) 
: FlyingCamera(input, startPos)
{
	m_bounds =  CBoundingBox(m_position + Vector3(0,-1.5,0), m_position + Vector3(1 ,2, 1));
}

Bullet* Player::SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture)
{
	Bullet* b = new Bullet("Player", mesh, shader, texture, m_lookAtTarget);
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

void Player::OnCollisionStay(GameObject* other)
{

}

void Player::OnCollisionEnter(GameObject* other)
{
	if(other->GetTag() == "ammo")
	{
		//Downcast from gameobject to ammoBox (I can't believe this worked!)
		AmmoBox *a = static_cast<AmmoBox *>(other);
		m_ammo += a->getBullets();
		a->Destroy();
		cout << "Picked up ammo!" << endl;
		return;
	}

	if (other->GetTag() == "Level")
	{
		ApplyForce(-localForwardXZ * 0.5f);

	}
}

void Player::OnCollisionExit(GameObject* other)
{

}

void Player::Update(float timeStep)
{
	m_position.y = 1.5f;

	//Update bounds
	m_bounds = CBoundingBox(m_position + Vector3(0, -1.5, 0), m_position + Vector3(2, 2, 2));

	//Update camera logic first
	FlyingCamera::Update(timeStep);

	//Reload
	if (m_input->GetKeyDown('R'))
	{
		if (m_inClip < m_clipCap && m_ammo > 0)
		{
			int ammoToTake = m_clipCap - m_inClip;

			if (ammoToTake > m_ammo)
				ammoToTake = m_ammo;

			m_inClip += ammoToTake;
			m_ammo -= ammoToTake;			
		}
	}

	//Shooty cooldown
	m_coolDown += timeStep * getSimSpeed(); //Update cooldown in relation to simTime

}


