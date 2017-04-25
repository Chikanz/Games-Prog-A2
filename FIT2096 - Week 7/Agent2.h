#pragma once
#include "Enemy.h"

class Agent2 : public Enemy 
{
public:
	Agent2(InputController* input, Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float timestep, float simSpeed);
	bool atTarget = false;

private:

};
