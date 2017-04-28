#include "Agent2.h"
#include "MathsHelper.h"


Agent2::Agent2(bool isRunner, Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
	: Enemy(player, mesh, shader, texture, position)
{
	m_runner = isRunner;
	m_moveSpeed = 0.1f;
	m_frictionAmount = 0.5f;
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

	//Move to target
	ApplyForce((m_target - m_position) * (m_moveSpeed * simSpeed));

	//Call parent
	Enemy::Update(timestep, simSpeed);	
}
