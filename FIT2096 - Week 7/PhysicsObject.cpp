#include "PhysicsObject.h"

PhysicsObject::PhysicsObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) : 
	GameObject(mesh, shader, texture, position)
{
	m_velocity = Vector3::Zero;
	m_acceleration = Vector3::Zero;

	m_angularVel = Vector3::Zero;
	m_angularAcc = Vector3::Zero;
}

void PhysicsObject::Update(float timestep, float simSpeed)
{
	// Apply friction every frame
	ApplyFriction(m_frictionAmount);

	// Acceleration trickles down into velocity
	m_velocity += m_acceleration;
	m_angularVel += m_angularAcc;

	// Velocity trickles down into position
	m_position += m_velocity * timestep * simSpeed;

	m_rotX += m_angularVel.x * timestep * simSpeed;
	m_rotY += m_angularVel.y * timestep * simSpeed;
	m_rotZ += m_angularVel.z * timestep * simSpeed;

	// Reset acceleration each frame so it doesn't accumulate
	m_acceleration = Vector3::Zero;
	m_angularAcc = Vector3::Zero;

}

void PhysicsObject::ApplyForce(Vector3 force)
{
	m_acceleration += force;
}


void PhysicsObject::ApplyTorque(Vector3 force)
{	
	m_angularAcc += force;
}

void PhysicsObject::Stop()
{
	m_angularVel = Vector3::Zero;;
	m_angularAcc = Vector3::Zero;
	m_acceleration = Vector3::Zero;
	m_velocity = Vector3::Zero;
}

void PhysicsObject::ApplyFriction(float strength)
{
	// Force in opposite direction to velocity
	ApplyForce(-m_velocity * strength);
	ApplyTorque(-m_angularVel * strength);
}
