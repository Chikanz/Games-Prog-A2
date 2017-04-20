#include "player.h"

Player::Player(InputController* input, Vector3 startPos) 
: FlyingCamera(input, startPos)
{

}

void Player::Update(float timeStep)
{
	//Update camera logic first
	FlyingCamera::Update(timeStep);

}

