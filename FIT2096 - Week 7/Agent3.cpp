//#include "Agent3.h"
//
//Agent3::Agent3(vector<Ruby*>* rubies, Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position)
//:Enemy(player, mesh, shader, texture, position)
//{
//	m_rubies = rubies;
//	m_moveSpeed = 0.1f;
//	m_frictionAmount = 0.5f;
//}
//
//void Agent3::Update(float timestep, float simSpeed)
//{
//	//Check if player took ruby
//	if((*m_rubies)[rubyIndex] == nullptr)
//	{
//		atTarget = true; 
//	}
//
//	if(atTarget)
//	{
//		atTarget = false;
//
//		//Make sure position isn't null
//		do {
//			rubyIndex = MathsHelper::RandomRange(0, m_rubies->size() - 1);
//		} 
//		while ((*m_rubies)[rubyIndex] == nullptr); 
//
//		m_target = (*m_rubies)[rubyIndex]->GetPosition(); //set target position 
//		m_target.y = 0;
//	}
//
//	//Move to target
//	ApplyForce((m_target - m_position) * (m_moveSpeed * simSpeed));
//
//	//Call parent
//	Enemy::Update(timestep, simSpeed);
//}
