#include "GameObject.h"
#include <iostream>
#include "MathsHelper.h"

GameObject::GameObject()
{
	m_position = Vector3::Zero;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_world = Matrix::Identity;
	m_mesh = NULL;
	m_texture = NULL;
	m_shader = NULL;
	id = MathsHelper::RandomRange(0, INT_MAX);
}
GameObject::GameObject(Mesh* mesh, Shader* shader)
{
	m_position = Vector3::Zero;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_world = Matrix::Identity;
	m_mesh = mesh;
	m_texture = NULL;
	m_shader = shader;
	id = MathsHelper::RandomRange(0, INT_MAX);
}
GameObject::GameObject(Mesh* mesh, Shader* shader, Texture* texture)
{
	m_position = Vector3::Zero;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_world = Matrix::Identity;
	m_mesh = mesh;
	m_texture = texture;
	m_shader = shader;
	id = MathsHelper::RandomRange(0, INT_MAX);
}
GameObject::GameObject(Mesh* mesh, Shader* shader, Vector3 position)
{
	m_position = position;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_world = Matrix::Identity;
	m_mesh = mesh;
	m_texture = NULL;
	m_shader = shader;
	id = MathsHelper::RandomRange(0, INT_MAX);
}
GameObject::GameObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
{
	m_position = position;
	m_rotX = m_rotY = m_rotZ = 0;
	SetUniformScale(1.0f);
	m_world = Matrix::Identity;
	m_mesh = mesh;
	m_texture = texture;
	m_shader = shader;
	id = MathsHelper::RandomRange(0, INT_MAX);
}

GameObject::~GameObject() {}

void GameObject::Render(Direct3D* renderer, Camera* cam)
{
	if (m_mesh)
	{
		// If creating these matrices every frame worries you, a "dirty" system like the camera uses could apply here too
		m_world = Matrix::CreateScale(m_scaleX, m_scaleY, m_scaleZ) * Matrix::CreateFromYawPitchRoll(m_rotY, m_rotX, m_rotZ) * Matrix::CreateTranslation(m_position);		
		m_mesh->Render(renderer, m_shader, m_world, cam, m_texture);
	}

}

//Leave these bad boys empty
void GameObject::OnCollisionStay(GameObject* other)
{
}

void GameObject::OnCollisionEnter(GameObject* other)
{
}

void GameObject::OnCollisionExit(GameObject* other)
{
}

void GameObject::GetShot()
{

}

void GameObject::updateBounds()
{
	m_bounds = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());
}
