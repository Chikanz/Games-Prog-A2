#include "FlyingCamera.h"
#include "MathsHelper.h"
#include <string>
#include <iostream>


FlyingCamera::FlyingCamera(InputController* input, Vector3 startPos)
{
	m_input = input;
	SetPosition(startPos);

	m_moveSpeed = 5.0f;
	m_heightChangeSpeed = 10.0f;
	m_rotationSpeed = 0.5f;

	m_heading = 0.0f;
	m_pitch = 0.0f;
}

void FlyingCamera::Update(float timestep)
{
	// Accumulate change in mouse position 
	m_heading += m_input->GetMouseDeltaX() * m_rotationSpeed * timestep;
	m_pitch += m_input->GetMouseDeltaY() * m_rotationSpeed * timestep;

	// Limit how far the player can look down and up
	m_pitch = MathsHelper::Clamp(m_pitch, ToRadians(-80.0f), ToRadians(80.0f));

	// Wrap heading and pitch up in a matrix so we can transform our look at vector
	// Heading is controlled by MouseX (horizontal movement) but it is a rotation around Y
	// Pitch  controlled by MouseY (vertical movement) but it is a rotation around X
	Matrix heading = Matrix::CreateRotationY(m_heading);
	Matrix pitch = Matrix::CreateRotationX(m_pitch);

	// Transform a world right vector from world space into local space
	Vector3 localRight = Vector3::TransformNormal(Vector3(1, 0, 0), heading);

	// Essentially our local forward vector but always parallel with the ground
	// Remember a cross product gives us a vector perpendicular to the two input vectors
	Vector3 localForwardXZ = localRight.Cross(Vector3(0, 1, 0));

	// We're going to need this a lot. Store it locally here to save on our function calls 
	Vector3 currentPos = GetPosition();

	//Change sim speed if buttons pressed
	m_simSpeed = m_slowSpeed;
	if (m_input->GetKeyHold('W'))
	{
		currentPos += localForwardXZ * m_moveSpeed * timestep;
		m_simSpeed = 1;
	}
	if (m_input->GetKeyHold('S'))
	{
		currentPos -= localForwardXZ * m_moveSpeed * timestep;
		m_simSpeed = 1;
	}
	if (m_input->GetKeyHold('A'))
	{
		currentPos -= localRight * m_moveSpeed * timestep;
		m_simSpeed = 1;
	}
	if (m_input->GetKeyHold('D'))
	{
		currentPos += localRight * m_moveSpeed * timestep;
		m_simSpeed = 1;
	}

	//If mouse is moving adjust sim speed slightly
	if ((m_input->GetMouseDeltaX() != 0 || m_input->GetMouseDeltaY()) && m_simSpeed < 1)
	{
		m_simSpeed = 0.04f;
	}

	std::cout << m_simSpeed << std::endl;

	// Combine pitch and heading into one matrix for convenience
	Matrix lookAtRotation = pitch * heading;

	// Transform a world forward vector into local space (take pitch and heading into account)
	Vector3 lookAt = Vector3::TransformNormal(Vector3(0, 0, 1), lookAtRotation);	

	// At this point, our look-at vector is still relative to the origin
	// Add our position to it so it originates from the camera and points slightly in front of it
	// Remember the look-at vector needs to describe a point in the world relative to the origin
	lookAt += currentPos;

	// Use parent's mutators so isDirty flags get flipped
	SetLookAt(lookAt);
	SetPosition(currentPos);	

	// Give our parent a chance to regenerate the view matrix
	Camera::Update(timestep);
}
