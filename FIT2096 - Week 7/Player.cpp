#include "player.h"
#include <iostream>
#include "MathsHelper.h"
#include "AmmoBox.h"
#include <algorithm>
#include "Enemy.h"
#include "CollisionManager.h"


//Oh yes
Player::Player(InputController* input, Vector3 startPos, Mesh* enemyMesh, CollisionManager* col)
//Player::Player(InputController* input, Vector3 startPos, Mesh* enemyMesh)
	: FlyingCamera(input, startPos),
	GameObject(enemyMesh, nullptr, nullptr, startPos)
{
	m_height = startPos.y;
	m_colliderMesh = enemyMesh;		
	cm = col;

	UpdateBounds();
}

Bullet* Player::SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture)
{
	Bullet* b = new Bullet(m_tag, mesh, shader, texture, m_lookAtTarget);
	//b->SetXRotation(m_pitch);
	b->SetYRotation(m_heading);
	return b;
}

bool Player::canFire()
{
	if (m_gun && m_gun->CanFire())
	{
		m_gun->Fire(ToRadians(-45));
		ForceSimSpeed(0.6f, 0.2f); //Speed up slightly like in super hot
		return true;
	}

	return false;
}

void Player::OnCollisionStay(GameObject* other)
{

}

void Player::OnCollisionEnter(GameObject* other)
{
	if (other->GetTag() == "Level")
	{
		ApplyForce(-localForwardXZ * 0.5f); //Make shift physics
	}	

	if (other->GetTag() == "Bullet")
	{
		hurtTimer = 0; //Trigger hurt animation
		m_health -= 1;

		if(m_health <= 0)
		{
			ShowCursor(true);
			MessageBox(0, "Game over!", "uh oh!", 0);
			PostQuitMessage(0);
		}
	}
}

void Player::OnCollisionExit(GameObject* other)
{

}

void Player::GrabGun(Gun* g)
{	
	m_gun = g;
	g->SetOwner(this);

}

void Player::ForceSimSpeed(float speed, float duration)
{
	forcedSimSpeed = speed;
	forceTimer = duration;
	forcingSpeed = true;
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
	outerBounds = CBoundingBox(m_position + meshMin - (meshMin * 0.5f), m_position + m_colliderMesh->GetMax() + (m_colliderMesh->GetMax() * 0.5f));
}

//Camera logic update
void Player::Update(float timestep)
{
	//Shaky gun!
	FlyingCamera::Update(timestep);

	//Force speed
	if (forcingSpeed)
	{
		m_simSpeed = forcedSimSpeed;
		forceTimer -= timestep;
		if (forceTimer <= 0.0f)
		{
			forcingSpeed = false;
		}
	}
}

//Regular logic update
void Player::Update(float timestep, float simSpeed) 
{	
	//Hurt animation
	if (hurtTimer < hurtDuration)
	{
		hurtTimer += timestep * getSimSpeed();
	}
	m_position.y = 1.5f;

	//Update bounds
	UpdateBounds();

	//World stuff
	m_position = m_camPosition;
	m_rotY = m_heading;
	m_rotX = m_pitch;
	m_world = Matrix::CreateScale(m_scaleX, m_scaleY, m_scaleZ) * lookAtRotation * Matrix::CreateTranslation(m_position);	

	//Throw gun
	if(m_input->GetMouseUp(1))
	{
		if(m_gun)
		{
			m_gun->RemoveOwner(m_lookAtTarget);
			m_gun = nullptr;
		}
	}

	//Grab gun
	if (m_input->GetMouseUp(0) && !m_gun)
	{
		Vector3 min = m_lookAtTarget + Vector3(-0.5f, 0, -1);
		Vector3 max = m_lookAtTarget + Vector3(0.5f, 0.5f, 1);
		if (cm->IsColliding(CBoundingBox(min, max), TriggerList)) //Get list of things colliding with
		{
			for (int i = 0; i < TriggerList->size(); i++)
			{
				GameObject* obj = (*TriggerList)[i];
				if (obj->GetTag() == "Gun")
				{
					Gun* g = static_cast<Gun *>(obj); //Cast to gun
					GrabGun(g);
					break;
				}
				if (obj->GetTag() == "Enemy")
				{
					Enemy* e = static_cast<Enemy *>(obj); //Cast to enemy
					if (e->IsDead()) continue;
					e->GetShot();
					break;
				}
			}
			if (TriggerList->size() > 0) TriggerList->clear();
		}
	}
}

void Player::Render(Direct3D* renderer, Camera* cam) //Nothing to render!
{

}


//CODE STORAGE
//This is basically the editor
//Debug

Vector3 Player::Editor(Vector3 input)
{
	if (m_input->GetKeyDown('I'))
		input.y += 0.1f;

	if (m_input->GetKeyDown('K'))
		input.y -= 0.1f;

	if (m_input->GetKeyDown('J'))
		input.x += 0.1f;

	if (m_input->GetKeyDown('L'))
		input.x -= 0.1f;


	if (m_input->GetKeyDown('Y'))
		input.z -= 0.1f;

	if (m_input->GetKeyDown('H'))
		input.z += 0.1f;

	cout << input.x << " " << input.y << " " << input.z << endl;

	return input;
}