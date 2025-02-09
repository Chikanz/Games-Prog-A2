#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Direct3D.h"
#include "Mesh.h"
#include "Collisions.h"

using namespace std;

class GameObject
{
protected:
	Vector3 m_position;
	float m_rotX, m_rotY, m_rotZ;
	float m_scaleX, m_scaleY, m_scaleZ;

	Matrix m_world;
	Mesh* m_mesh;
	Texture* m_texture;
	Shader* m_shader;

	CBoundingBox m_bounds;

	string m_tag = "";
	bool destroyMarked = false;

	int id; //Unique ID for comparison (not validated)

public:
	GameObject();
	GameObject(Mesh* mesh, Shader* shader);
	GameObject(Mesh* mesh, Shader* shader, Texture* texture);
	GameObject(Mesh* mesh, Shader* shader, Vector3 position);
	GameObject(Mesh* mesh, Shader* shader, Texture* texture, Vector3 position);
	virtual ~GameObject();

	//GameObject is now an abstract class as Update is pure virtual
	virtual void Update(float timestep, float simTime) = 0;
	virtual void Render(Direct3D* renderer, Camera* cam);

	//Collisions
	virtual void OnCollisionStay(GameObject* other);
	virtual void OnCollisionEnter(GameObject* other);
	virtual void OnCollisionExit(GameObject* other);

	virtual void GetShot();

	// Accessors
	Vector3 GetPosition() { return m_position; }
	float GetXRotation() { return m_rotX; }
	float GetYRotation() { return m_rotY; }
	float GetZRotation() { return m_rotZ; }
	float GetXScale() { return m_scaleX; }
	float GetYScale() { return m_scaleY; }
	float GetZScale() { return m_scaleZ; }
	Mesh* GetMesh() { return m_mesh; }
	Texture* GetTexture() { return m_texture; }
	Shader* GetShader() { return m_shader; }
	string GetTag() { return m_tag; };

	Matrix getWorld() { return m_world; };

	CBoundingBox GetBounds() { return m_bounds; };
	virtual void UpdateBounds();
	bool MarkedForDestroy() { return destroyMarked; };

	// Mutators
	void SetTag(string tag) { m_tag = tag; }
	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetXRotation(float xRot) { m_rotX = xRot; }
	void SetYRotation(float yRot) { m_rotY = yRot; }
	void SetZRotation(float zRot) { m_rotZ = zRot; }
	void SetXScale(float xScale) { m_scaleX = xScale; }
	void SetYScale(float yScale) { m_scaleY = yScale; }
	void SetZScale(float zScale) { m_scaleZ = zScale; }
	void SetUniformScale(float scale) { m_scaleX = m_scaleY = m_scaleZ = scale; }
	void SetMesh(Mesh* mesh) { m_mesh = mesh; }
	void SetTexture(Texture* texture) { m_texture = texture; }
	void SetShader(Shader* shader) { m_shader = shader; }

	void Destroy() { destroyMarked = true; };
};

#endif

