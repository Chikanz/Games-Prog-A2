#include "Gun.h"
#include <ostream>
#include <iostream>
#include "MathsHelper.h"

Gun::Gun(InputController* input, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
	: PhysicsObject(mesh, shader, texture, position)
{
	m_input = input;
	updateBounds();
}

void Gun::SetOwner(GameObject* newOwner)
{
	m_owner = newOwner;

	if (newOwner->GetTag() == "Enemy")	
		grabPos = enemyGrabPos;
	else
		grabPos = playerGrabPos;

	m_velocity = Vector3::Zero;
	m_position = grabPos;
}

void Gun::RemoveOwner()
{	
	if (m_owner == nullptr) return;

	//Transform to world space
	m_position = grabPos + m_owner->GetPosition();
	m_rotX = m_owner->GetXRotation();
	m_rotY = m_owner->GetYRotation();
	m_rotZ = m_owner->GetZRotation();

	//Get local forward and throw
	Matrix heading = Matrix::CreateRotationY(m_rotY) * Matrix::CreateRotationX(m_rotX);
	Vector3 localForward = Vector3::TransformNormal(Vector3(0,0,1), heading);	
	ApplyForce(localForward * 3 + (Vector3(0,1,0) * 5));

	m_owner = nullptr;
	grabPos = Vector3::Zero;
}

void Gun::Update(float timestep, float simSpeed)
{	
	if(!m_owner) //If not grabbed
	{
		if (m_position.y + GetBounds().GetMax().y > 0)
			ApplyForce(Vector3(0, -0.05f, 0)); //Gravity
		else
			m_velocity = Vector3::Zero;

		m_position.y = MathsHelper::Clamp(m_position.y, 0, 999);

		PhysicsObject::Update(timestep, simSpeed);
	}

	updateBounds();
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
