#include "Agent2.h"
#include "MathsHelper.h"


Agent2::Agent2(InputController* input, Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
	: Enemy(input, player, mesh, shader, texture, position)
{
	m_moveSpeed = 0.005f;
}

void Agent2::Update(float timestep, float simSpeed)
{
	atTarget = Vector3::Distance(m_position, m_target) < 0.1f;

	if (atTarget) //Set target when reached
	{
		m_velocity = Vector3::Zero;
		//Traverse using current position, then clamp
		m_target = Vector3(MathsHelper::RandomRange(-5, 5) + m_position.x, 0, MathsHelper::RandomRange(-5, 5) + m_position.z);
		m_target.Clamp(Vector3(-20, -20, -20), Vector3(20, 20, 20));
	}		

	//Call parent update to rotate
	Enemy::Update(timestep, simSpeed);

	//Move and Update physics
	if(!debugMode)
		ApplyForce((m_target - m_position) * m_moveSpeed);
	PhysicsObject::Update(timestep, simSpeed);	
}
