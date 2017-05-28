/*	FIT2096 - Example Code
*	FlyingCamera.h
*	Created by Mike Yeates - 2016 - Monash University
*	A flying camera can move up and down, look around freely with
*	the mouse, and always moves parallel to the ground
*/

#ifndef FLYING_CAMERA_H
#define FLYING_CAMERA_H

#include "Camera.h"
#include "InputController.h"
#include "GameObject.h"
#include "bullet.h"

class FlyingCamera : public Camera
{
protected:
	InputController* m_input;

	float m_moveSpeed;
	float m_rotationSpeed;
	float m_heightChangeSpeed;

	float cursorDelta;
	float resetThreshold = 100;

	float m_simSpeed = 1.0f;
	const float m_slowSpeed = 0.013f;
	const float m_fastSpeed = 0.8f;

	Vector3 localForwardXZ; //Use the local forward for collision
	Vector3 lookAt;

	//Physics
	Vector3 m_acceleration;
	Vector3 m_velocity;
	float m_frictionAmount = 0.5f;
	const float m_moveFriction = 0.5f;
	const float m_slideFriction= 0.2f;

	const float m_moveScale = 0.04f; //Velocity magnitude always seems to be twice this	

	float m_heading;
	float m_pitch;

	bool canMove = true;

	Matrix lookAtRotation;
	

public:
	FlyingCamera(InputController* input, Vector3 startPos);
	void Update(float timestep);
	void ApplyForce(Vector3 force);
	void ApplyFriction(float strength);

	void Freeze(bool t) { canMove = !t;};
};

#endif