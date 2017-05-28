#include "player.h"
#include <iostream>
#include "MathsHelper.h"
#include "AmmoBox.h"
#include <algorithm>
#include "Enemy.h"
#include "CollisionManager.h"
#include "TextMan.h"


/*I used a multiple inheritance player so that it can be both a player and a game object. This means it can be used
 *as both a camera and a game object, allowing for a more simplified collision system among other things.
*/
Player::Player(InputController* input, Vector3 startPos, Mesh* enemyMesh, CollisionManager* col, AudioSystem* _AS)

	: FlyingCamera(input, startPos),
	GameObject(enemyMesh, nullptr, nullptr, startPos)
{
	AS = _AS;
	m_startPos = startPos;
	m_height = startPos.y;
	m_colliderMesh = enemyMesh;		
	cm = col;
	SetTag("Player");
	UpdateBounds();
	
}

Bullet* Player::SpawnBullet(Mesh* mesh, Shader* shader, Texture* texture)
{
	Bullet* b = new Bullet(m_tag, mesh, shader, texture, m_lookAtTarget);
	//b->SetXRotation(m_pitch); uhhh
	b->SetYRotation(m_heading);
	return b;
}

bool Player::canFire()
{
	if (!m_gun) return false;

	if (m_gun->CanFire())
	{
		m_gun->Fire(ToRadians(-45));
		ForceSimSpeed(0.6f, 0.2f); //Speed up slightly like in super hot
		AudioClip* clip = AS->Play("Assets/Sounds/shotshot.wav", false);
		clip->SetVolume(0.8f);		
		return true;
	}

	if(m_gun->GetAmmo() <= 0)
	{
		TextMan* TM = TextMan::GetTextMan();

		TM->AddText("ALL", 1);
		TM->AddText("OUT", 1);

		AudioClip* clip = AS->Play("Assets/Sounds/emptyammo.wav", false);
		clip->SetVolume(1);

		ThrowGun();
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
		ApplyForce(-m_velocity * 4); //Makeshift physics
	}	
}

void Player::OnCollisionExit(GameObject* other)
{

}

void Player::GetShot()
{
	hurtTimer = 0; //Trigger hurt animation
	m_health -= 1;
}

void Player::TriggerOverlay()
{
	hurtTimer = 0; //Trigger hurt animation
}

void Player::GrabGun(Gun* g)
{	
	AudioClip* clip = AS->Play("Assets/Sounds/gunPickup.wav", false);
	clip->SetVolume(0.8f);

	if(g->SetOwner(this))
		m_gun = g;
}

void Player::ThrowGun()
{
	if (m_gun)
	{
		AudioClip* clip = AS->Play("Assets/Sounds/gunThrowaway.wav", false);
		clip->SetVolume(0.8f);

		m_gun->RemoveOwner(m_lookAtTarget);
		m_gun = nullptr;
	}
}

void Player::Reset()
{		
	m_gun = nullptr;
	m_camPosition = m_startPos;
	
	m_health = 1;

	m_acceleration = Vector3::Zero;
	m_velocity = Vector3::Zero;

	//Flying cam reset
	m_heading = ToRadians(30);
	m_pitch = 0.0f;
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
	Vector3 meshMax = m_colliderMesh->GetMax();
	meshMax.y = 0;
	meshMin.y = -0.5f;
	m_bounds = CBoundingBox(m_position + meshMin, m_position + meshMax);
}

//Camera logic update
void Player::Update(float timestep)
{

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

	//Calc floor
	//all floor must be plane

	//Jump (experimental)
	if (m_input->GetKeyDown(' ') && !m_inAir)
	{
		ApplyForce(Vector3::Up * 0.5f);
		m_inAir = true;
	}

	//Gravity
	if (m_camPosition.y > m_height + m_floor)
	{
		ApplyForce(Vector3(0, -0.03f, 0)); //Gravity?
		m_inAir = true;
	}		
	else
	{
		m_inAir = false;
	}
	m_camPosition.y = MathsHelper::Clamp(m_camPosition.y, m_height + m_floor, 20);

	//Shaky gun + update super
	FlyingCamera::Update(timestep);

}

//Regular logic update
void Player::Update(float timestep, float simSpeed) 
{	
	//Audio Listener
	Vector3 up = lookAt.Cross(Vector3(0, 1, 0));
	AS->SetListener3DAttributes(m_camPosition, localForwardXZ, Vector3(0, 1, 0), Vector3::Zero);

	//Hurt animation
	if (hurtTimer < hurtDuration)
	{
		hurtTimer += timestep * getSimSpeed();
	}


	//Update bounds
	UpdateBounds();

	//World stuff
	m_position = m_camPosition; //DON'T TOUCH m_position! Read only value!
	m_rotY = m_heading;
	m_rotX = m_pitch;
	m_world = Matrix::CreateScale(m_scaleX, m_scaleY, m_scaleZ) * lookAtRotation * Matrix::CreateTranslation(m_position);	

	//Throw gun
	if(m_input->GetMouseUp(1))
	{
		ThrowGun();
	}

	//Grab gun
	if (m_input->GetMouseUp(0) && !m_gun)
	{
		//So this is a bit of a hack
		Vector3 min = m_lookAtTarget + Vector3(-1, -1, -1);
		Vector3 max = m_lookAtTarget + Vector3(1, 1, 1);


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
					AudioClip* clip = AS->Play("Assets/Sounds/neckcrack1.wav", false);
					clip->SetVolume(0.8f);

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