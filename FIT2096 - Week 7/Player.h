#pragma once
#include "FlyingCamera.h"

class Player : public FlyingCamera
{
private:	
	float health;
	float bullets;
	float bulletTimer; 

public:
	void Update(float timeStep);
	Player(InputController* input, Vector3 startPos);

	Vector3 getHeading() { return m_lookAtTarget; };
	float getSimSpeed() { return m_simSpeed; };
};
