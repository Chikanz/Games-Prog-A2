#include "Agent1.h"
#include <iostream>

Agent1::Agent1(Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
	: Enemy(player, mesh, shader, texture, position)
{
	m_moveSpeed = 0.07f;
	m_frictionAmount = 0.5f;
}

void Agent1::Update(float timestep, float simSpeed)
{
	//Get vector to player and move
	m_target = m_player->GetPosition();
	m_target.y = 0;
	ApplyForce((m_target - m_position) * (m_moveSpeed * simSpeed));	

	//Call parent
	Enemy::Update(timestep, simSpeed);	
}
