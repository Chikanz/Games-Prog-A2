#ifndef PHYSICS_OBJECT_H
#define PHYSICS_OBJECT_H

#include "GameObject.h"

class PhysicsObject : public GameObject
{
protected:
	// Implementing basic physics
	// http://natureofcode.com/book/chapter-2-forces/

	// Parent manages position vector
	Vector3 m_velocity;
	Vector3 m_acceleration;

	float m_frictionAmount;

	void ApplyForce(Vector3 force);
	void ApplyFriction(float strength);

	Vector3 m_angularVel = Vector3::Zero;
	Vector3 m_angularAcc = Vector3::Zero;

public:
	PhysicsObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	virtual void Update(float timestep, float simSpeed); //Must call from child to update physics

	Vector3 GetVelocity() { return m_velocity; }

	void ApplyTorque(Vector3 force);
	void Stop();

};

#endif