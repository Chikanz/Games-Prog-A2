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
	Bullet* b = new Bullet(m_tag, mesh, shader, texture, m_lookAtTarget);
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
		//Downcast from gameobject to ammoBox
		AmmoBox *a = static_cast<AmmoBox *>(other);

		m_ammo += a->getBullets();
		a->Destroy();
		return;
	}

	if (other->GetTag() == "Level")
	{
		ApplyForce(-localForwardXZ * 0.5f); //Make shift physics
	}

	if(other->GetTag() == "Ruby")
	{
		rubiesHeld += 1;
		other->Destroy();

		if(rubiesHeld == maxRubies) //game over
		{
			MessageBox(0, 0, "You win!", 0);
			PostQuitMessage(0);
		}
	}

	if (other->GetTag() == "Bullet")
	{
		hurtTimer = 0; //Trigger hurt animation
		m_health -= 1;
		if(m_health <= 0)
		{
			MessageBox(0, "uh oh!", "Game over!", 0);
			PostQuitMessage(0);
		}
		
	}
}

void Player::OnCollisionExit(GameObject* other)
{

}

float Player::GetHurtAlpha()
{
	if(hurtTimer < hurtDuration)
	{
		return  MathsHelper::RemapRange(hurtTimer, 0.0f, hurtDuration, 1.0f, 0.0f);		
	}
	return 0;
}

void Player::UpdateBounds()
{
	Vector3 meshMin = m_colliderMesh->GetMin();
	meshMin.y -= m_height;
	m_bounds = CBoundingBox(m_position + meshMin, m_position + m_colliderMesh->GetMax());
}

void Player::Update(float timeStep)
{
	if (hurtTimer < hurtDuration)
	{
		hurtTimer += timeStep * getSimSpeed();
	}

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


