#include "TextMan.h"
#include <sstream>
#include <iostream>
#include "AudioClip.h"

TextMan *TextMan::TM; //C++ is weird

TextMan* TextMan::GetTextMan(AudioSystem* AS)
{
	if (!TM) TM = new TextMan(AS);
	return TM;
}

TextMan* TextMan::GetTextMan()
{
	return TM;
}

wstring TextMan::getText()
{
	wstringstream ss;
	ss << current.c_str();
	return ss.str();
}

void TextMan::AddText(string s, float interval)
{
	if(toDisplay.size() <= 0)
	{
		AudioClip* clip = AS->Play("Assets/Sounds/migawka.wav", false);
		clip->SetVolume(1);
		clip->TimeShift = false;
	}

	Message m;
	m.text = s;
	m.interval = interval;

	toDisplay.push(m);
	timer = 0;	
}

void TextMan::Update(float timestep)
{		
	if (toDisplay.size() <= 0) //If nothing in queue dw
	{
		current = "";
	}
	else if(timer > displayTime) //Timer tick
	{
		toDisplay.pop();
		if(toDisplay.size() > 0)
		{
			timer = 0;		

			if(toDisplay.front().text == "SUPER")
			{
				AudioClip* clip = AS->Play("Assets/Sounds/super2.wav", false);
				clip->SetVolume(1);
				clip->TimeShift = false;
			}
			else if(toDisplay.front().text == "HOT")
			{
				AudioClip* clip = AS->Play("Assets/Sounds/hot2.wav", false);
				clip->SetVolume(1);
				clip->TimeShift = false;
			}
			else
			{
				AudioClip* clip = AS->Play("Assets/Sounds/migawka.wav", false);
				clip->SetVolume(1);
				clip->TimeShift = false;
			}

		}
			
	}
	else //There's stuff in queue, count down timer
	{
		timer += timestep;
		current = toDisplay.front().text;
		displayTime = toDisplay.front().interval;
	}	
}

TextMan::TextMan(AudioSystem* _AS)
{	
	AS = _AS;
	//current = toDisplay.front();
}
