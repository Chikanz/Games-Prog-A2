#pragma once
#include <string>
#include <queue>
#include "AudioSystem.h"
#include "AudioClip.h"
using namespace std;

//Text manager as a singleton that handles all of the on screen text flashes

struct Message
{
	string text;
	float interval;
};

class TextMan
{
public:
	static TextMan* GetTextMan(AudioSystem* _AS);
	static TextMan* GetTextMan();

	wstring getText();
	void AddText(string s, float interval);
	void Update(float timestep);	
	void Clear() { toDisplay.empty(); };

private:
	TextMan(AudioSystem* _AS);
	AudioSystem* AS;
	static TextMan* TM;
	float timer = 0;
	float displayTime = 1.0f;
	queue<Message> toDisplay;

	string past;
	string current;

	

};
