#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <vector>
#include "Collisions.h"
#include "Kart.h"
#include "ItemBox.h"
#include "player.h"

#define MAX_ALLOWED_COLLISIONS 2048

class CollisionManager
{
private:
	Player* m_player;
	GameObject* m_playerObj; //fake player game object to make manager happy
	std::vector<GameObject*>* m_sceneObjects;

	GameObject* m_currentCollisions[MAX_ALLOWED_COLLISIONS];

	// We need to know what objects were colliding last frame so we can determine if a collision has just begun or ended
	GameObject* m_previousCollisions[MAX_ALLOWED_COLLISIONS];

	int m_nextCurrentCollisionSlot;

	// Check if we already know about two objects colliding
	bool ArrayContainsCollision(GameObject* arrayToSearch[], GameObject* first, GameObject* second);

	// Register that a collision has occurred
	void AddCollision(GameObject* first, GameObject* second);

	// Collision check helpers
	void PlayerToScene();
	void SceneToScene();
	//void KartToKart();

public:
	CollisionManager(Player* player, GameObject* dummyPlayer, std::vector<GameObject*>* sceneObjects);
	void CheckCollisions();

};

#endif