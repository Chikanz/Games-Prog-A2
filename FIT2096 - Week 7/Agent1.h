#pragma once
#include "Enemy.h"
//#include "badPun.h"

class Agent1 : public Enemy //No. 1
{
public:
	Agent1(Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float timestep, float simSpeed);
};
