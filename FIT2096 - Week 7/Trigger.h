#pragma once
#include "GameObject.h"
#include <vector>
#include "TextMan.h"
#include "player.h"

//Simple class that triggers when the player is near for text display

class Trigger
{
public:
	Trigger(Player* p, Vector3 pos, float displayTime, string sentence, TextMan* TM);
	void Update();


private:
	vector<string> strings;
	void Payload();
	TextMan* TM;
	float displayTime;
	Player* player;
	bool triggered = false;

	Vector3 position;
};


