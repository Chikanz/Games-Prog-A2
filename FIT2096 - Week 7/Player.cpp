#include "player.h"
#include <iostream>
#include "MathsHelper.h"
#include "AmmoBox.h"

Player::Player(InputController* input, Vector3 startPos, Mesh* enemyMesh) 
: FlyingCamera(input, startPos)
{
	m_height = startPos.y;
	m_colliderMesh = enemyMesh;
	UpdateBounds();
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

	if(other->GetTag() == "Ruby")
	{
		rubiesHeld += 1;
		other->Destroy();
	}
}

void Player::OnCollisionExit(GameObject* other)
{

}

void Player::UpdateBounds()
{
	Vector3 meshMin = m_colliderMesh->GetMin();
	meshMin.y -= m_height;
	m_bounds = CBoundingBox(m_position + meshMin, m_position + m_colliderMesh->GetMax());
}

void Player::Update(float timeStep)
{
	m_position.y = 1.5f;

	//Update bounds
	UpdateBounds();

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


