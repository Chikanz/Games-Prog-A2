#include "Gun.h"
#include <ostream>
#include <iostream>
#include "MathsHelper.h"

Gun::Gun(InputController* input, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
	: PhysicsObject(mesh, shader, texture, position)
{
	m_input = input;
	UpdateBounds();
	SetTag("Gun");
}

void Gun::SetOwner(GameObject* newOwner)
{
	m_owner = newOwner;
	
	playerOwned = true;
	if (newOwner->GetTag() == "Enemy")
	{
		grabPos = enemyGrabPos;
		playerOwned = false;
	}
	else
	{
		grabPos = playerGrabPos;
	}

	m_velocity = Vector3::Zero;
	m_position = grabPos;	
}

void Gun::RemoveOwner(Vector3 worldPos)
{	
	if (m_owner == nullptr) return;

	if (playerOwned)
	{
		//Spawn at m_lookat, then offset locally (THIS ALMOST WORLKS PERFECTLY!)
		Vector3 offset = playerGrabPos;
		offset.z = 0;
		Vector3 localOffset = Vector3::TransformNormal(offset, Matrix::CreateRotationY(m_owner->GetYRotation()));
		m_position = worldPos + localOffset;
	}
	else //Enemy
	{
		m_position = grabPos + m_owner->GetPosition();
	}

	//Common, set rot 
	m_rotX = m_owner->GetXRotation();
	m_rotY = m_owner->GetYRotation();
	m_rotZ = m_owner->GetZRotation();

	//Throw that spicy boy
	Matrix heading = Matrix::CreateRotationY(m_rotY) * Matrix::CreateRotationX(m_rotX);
	Vector3 localForward = Vector3::TransformNormal(Vector3(0,0,1), heading);	
	ApplyForce(localForward * 4 + (Vector3(0,1,0) * 3));
	ApplyTorque(Vector3(MathsHelper::RandomRange(0.0f, 4.0f),	//Forward, always positive so we get a nice forward roll
		MathsHelper::RandomRange(-1.0f, 1.0f),					//A dash of this to make it interesting
		MathsHelper::RandomRange(-3.0f, 3.0f)					//Which way the gun barrel rolls
	));

	playerOwned = false;
	m_owner = nullptr;
	grabPos = Vector3::Zero;
	
}

void Gun::Update(float timestep, float simSpeed)
{	

	cout << m_rotX << endl;
	if(!m_owner) //If not grabbed
	{
		if (m_position.y + GetBounds().GetMax().y > 0)
			ApplyForce(Vector3(0, -0.1f, 0) * simSpeed); //Gravity?
		else
		{
			m_velocity = Vector3::Zero;			
		}		
	}
	else
	{
		if (playerOwned)
		{
			if (m_rotX < 0)
			{
				ApplyTorque(Vector3(0.01f * 20 * simSpeed, 0, 0));
			}
			else
			{
				m_angularVel = Vector3::Zero;
			}
		}
	}


	PhysicsObject::Update(timestep, simSpeed);
	UpdateBounds();
}

void Gun::Render(Direct3D* renderer, Camera* cam)
{
	if (m_mesh)
	{
		m_world = Matrix::CreateScale(m_scaleX, m_scaleY, m_scaleZ) * Matrix::CreateFromYawPitchRoll(m_rotY, m_rotX, m_rotZ) * Matrix::CreateTranslation(m_position);
		if(m_owner != nullptr)
			m_mesh->Render(renderer, m_shader, m_world * m_owner->getWorld(), cam, m_texture); //Child the gun to the owner
		else
			m_mesh->Render(renderer, m_shader, m_world, cam, m_texture); //Render normally
	}
}

void Gun::KnockBack(float force)
{
	m_rotX = 0;
	ApplyTorque(Vector3(force * 5, 0, 0));
}
