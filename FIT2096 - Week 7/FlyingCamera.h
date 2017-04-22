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

	float m_simSpeed = 1.0f;
	const float m_slowSpeed = 0.005f;

public:
	FlyingCamera(InputController* input, Vector3 startPos);
	void Update(float timestep);

	//TODO: getters + setters
	float m_heading;
	float m_pitch;
};

#endif