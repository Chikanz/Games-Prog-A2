#pragma once
#include <string>
#include <queue>
using namespace std;

//Text manager as a singleton

struct Message
{
	string text;
	float interval;
};

class TextMan
{
public:
	static TextMan* GetTextMan();

	wstring getText();
	void AddText(string s, float interval);
	void Update(float timestep);	
	void Clear() { toDisplay.empty(); };

private:
	TextMan();
	static TextMan* TM;
	float timer = 0;
	float displayTime = 1.0f;
	queue<Message> toDisplay;

	string current;

	

};
