#pragma once

#include "GameObject.h"

//Basically a skeleton gameobject to store bounds, so we can whack it in with the rest of the gameobjects

class StaticBounds : public GameObject
{
public:
	StaticBounds(CBoundingBox b) : GameObject() { m_bounds = b; };
	void Print();

	//Don't render or update since level is already drawn in it's own local space
	void Update(float timestep, float simTime)
	{

	}
	
	void Render()
	{
		
	}

};

inline void StaticBounds::Print()
{
	Vector3 min = m_bounds.GetMin();
	Vector3 max = m_bounds.GetMax();
	cout << min.x << " " << min.y << " " << min.z << " " << endl;
	cout << max.x << " " << max.y << " " << max.z << " " << endl << endl;	
}
