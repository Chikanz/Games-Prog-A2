#include "TextMan.h"
#include <sstream>
#include <iostream>

TextMan *TextMan::TM; //C++ is weird

TextMan* TextMan::GetTextMan()
{
	TM = NULL;
	if (!TM) TM = new TextMan();
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
		}
	}
	else //There's stuff in queue, count down timer
	{
		timer += timestep;
		current = toDisplay.front().text;
		displayTime = toDisplay.front().interval;
	}
}

TextMan::TextMan()
{	
	//current = toDisplay.front();
}
