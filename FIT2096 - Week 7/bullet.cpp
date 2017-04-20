#include "bullet.h"


bullet::bullet(Vector3 heading, Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) :
GameObject(mesh, shader, texture, position)
{
	m_bounding = CBoundingBox(m_position + m_mesh->GetMin(), m_position + m_mesh->GetMax());;
	m_heading = heading;
}

void bullet::Update(float timeStep, float simTime)
{
	//m_position += m_heading * m_bulletSpeed * timeStep * simTime;
	//m_position.z += m_bulletSpeed * timeStep * simTime;

	// We'll start by declaring a forward vector in world space
	Vector3 worldForward = Vector3(0, 0, 1);

	// Next we'll wrap up our Y rotation in a matrix (remember matrices transform vectors)
	Matrix heading = Matrix::CreateRotationY(m_rotY);

	// Finally, we'll transform our world forward vector by the heading matrix which 
	// will essentially spin it from a world forward into a local forward which takes
	// the object's rotation into account.
	Vector3 localForward = Vector3::TransformNormal(worldForward, heading);
	m_position += localForward * m_bulletSpeed * timeStep * simTime;
}

