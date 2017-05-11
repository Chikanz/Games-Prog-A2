#include "Ruby.h"
#include "MathsHelper.h"

Ruby::Ruby(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position) 
: GameObject(mesh, shader, texture, position)
{
	UpdateBounds();
	m_tag = "Ruby";

	m_rotY = ToRadians(MathsHelper::RandomRange(0, 360));
}

void Ruby::Update(float timeStep, float simSpeed)
{
	UpdateBounds();
	m_rotY += m_spinSpeed * simSpeed * timeStep;
}

