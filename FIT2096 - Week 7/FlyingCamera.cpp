#include "FlyingCamera.h"
#include "MathsHelper.h"
#include <string>
#include <iostream>


FlyingCamera::FlyingCamera(InputController* input, Vector3 startPos)
{
	m_input = input;
	SetPosition(startPos);

	m_moveSpeed = 3.0f;
	m_heightChangeSpeed = 10.0f;
	m_rotationSpeed = 0.5f;

	m_heading = ToRadians(30);
	m_pitch = 0.0f;
}

void FlyingCamera::Update(float timestep)
{
	// Add mouse deltas
	m_heading += m_input->GetMouseDeltaX() * m_rotationSpeed * timestep;
	m_pitch += m_input->GetMouseDeltaY() * m_rotationSpeed * timestep;

	cursorDelta += abs(m_input->GetMouseDeltaX()) + abs(m_input->GetMouseDeltaY());
	if(cursorDelta > resetThreshold)
	{
		SetCursorPos(990, 540);
		cursorDelta = 0;
	}

	// Look clamp
	m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f));

	// Pitch and heading to matrix
	Matrix heading = Matrix::CreateRotationY(m_heading);
	Matrix pitch = Matrix::CreateRotationX(m_pitch);

	// Get local right from world
	Vector3 localRight = Vector3::TransformNormal(Vector3(1, 0, 0), heading);
	//Vector3 localRight = Vector3::TransformNormal(Vector3(1, 0, 0), Matrix::CreateRotationY(m_heading));

	// Essentially our local forward vector but always parallel with the ground
	// Remember a cross product gives us a vector perpendicular to the two input vectors
	localForwardXZ = localRight.Cross(Vector3(0, 1, 0));

	//Input
	Vector3 moveForce;
	m_frictionAmount = 0.2f;
	if (m_input->GetKeyHold('W'))
	{
		moveForce += localForwardXZ * timestep;				
		m_frictionAmount = m_moveFriction;
	}
	if (m_input->GetKeyHold('S'))
	{
		moveForce += -localForwardXZ * timestep;						
		m_frictionAmount = m_moveFriction;
	}
	if (m_input->GetKeyHold('A'))
	{
		moveForce += -localRight * timestep;						
		m_frictionAmount = m_moveFriction;
	}
	if (m_input->GetKeyHold('D'))
	{
		moveForce += localRight  * timestep;		
		m_frictionAmount = m_moveFriction;
	}		

	//Normalize move force so diagnals are the same speed
	moveForce.Normalize();
	moveForce *= m_moveScale; 
	ApplyForce(moveForce);

	//Physics! (after we have move speed)
	ApplyFriction(m_frictionAmount);
	m_velocity += m_acceleration;
	m_camPosition += m_velocity;
	m_acceleration = Vector3::Zero;

	//Map max velocity (0 to twice movescale) to simSpeed (slowest sim speed to 1)
	m_simSpeed = MathsHelper::RemapRange(m_velocity.Length(), 0, m_moveScale * 2, m_slowSpeed, m_fastSpeed);
	

	//Update camera position after physics is calculated
	Vector3 currentPos = GetCamPosition();

	// Combine pitch and heading into one matrix for convenience
	lookAtRotation = pitch * heading;

	// Transform a world forward vector into local space (take pitch and heading into account)
	Vector3 lookAt = Vector3::TransformNormal(Vector3(0, 0, 1), lookAtRotation);

	// At this point, our look-at vector is still relative to the origin
	// Add our position to it so it originates from the camera and points slightly in front of it
	// Remember the look-at vector needs to describe a point in the world relative to the origin
	lookAt += currentPos;

	//If mouse is moving adjust sim speed slightly
	if ((m_input->GetMouseDeltaX() != 0 || m_input->GetMouseDeltaY()) && m_simSpeed < m_fastSpeed)
	{
		m_simSpeed += m_slowSpeed * 5;
	}	

	m_simSpeed = MathsHelper::Clamp(m_simSpeed, m_slowSpeed, m_fastSpeed); //Make sure no funny business is going on 

	SetLookAt(lookAt);
	SetPosition(currentPos);	

	// Call super to update matrix
	Camera::Update(timestep);
}

void FlyingCamera::ApplyForce(Vector3 force)
{
	// You could take mass into account here if you want to get fancy		
	m_acceleration += force;
}

void FlyingCamera::ApplyFriction(float strength)
{
	// Force in opposite direction to velocity
	ApplyForce(-m_velocity * strength);
}

