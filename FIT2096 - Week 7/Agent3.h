#pragma once
#include "Enemy.h"
#include <vector>
#include "Ruby.h"

class Agent3 : public Enemy
{
public:
	Agent3(vector<Ruby*>* rubies ,Player* player, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	void Update(float timestep, float simSpeed);
	bool atTarget = true;

private:
	vector<Ruby*>* m_rubies;
	int rubyIndex;

};

