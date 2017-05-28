#include "Trigger.h"
#include <iterator>
#include <sstream>

//taken from
//https://stackoverflow.com/questions/236129/split-a-string-in-c
template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}


Trigger::Trigger(Player* p, Vector3 pos, float dt, string text, TextMan* tm)	
{	
	strings = split(text, ' ');
	TM = tm;
	displayTime = dt;
	player = p;
	position = pos;
}

void Trigger::Update()
{
	if (triggered) return;	
	if (Vector3::Distance(player->GetPosition(), position) < 2)
	{
		Payload();
	}
}


void Trigger::Payload()
{
	if (triggered) return;

	triggered = true;
	for (int i = 0; i < strings.size(); i++)
	{
		TM->AddText(strings[i], displayTime);
	}
}



