//File reading
//http://stackoverflow.com/questions/7868936/read-file-line-by-line
//String parsing 
//http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c

#pragma once
#include "Collisions.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

static struct EnemyInfo
{
	float Xpos;
	float Ypos;
	float Zpos;

	float YRot;
	Enemy::eAction action;
};

class FileReader
{
public:
	FileReader();
	~FileReader();
	vector<CBoundingBox>* ReadBounds();
	vector<EnemyInfo>* ReadEnemies();

	

private:
	vector<CBoundingBox>* boundsList = new vector<CBoundingBox>;
	vector<EnemyInfo>* enemyInfoList = new vector<EnemyInfo>;
};


FileReader::FileReader()
{
	
}

FileReader::~FileReader()
{
	//boundsList->clear();
	//enemyInfoList->clear();
}

inline vector<CBoundingBox>* FileReader::ReadBounds()
{
	ifstream infile("Assets/LevelBounds.Zac");
	string line;
	string d = ", "; //Delimiter

	float bounds[6];
	while (getline(infile, line))
	{
		string s = line;
		string delimiter = ", ";

		size_t pos = 0;
		string token;
		string::size_type sz; //keep stof happy
		int i = 0;
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			bounds[i] = stof(token, &sz);
			s.erase(0, pos + delimiter.length());
			i++;
		}

		Vector3 min = Vector3(bounds[0], bounds[1], bounds[2]);
		Vector3 max = Vector3(bounds[3], bounds[4], bounds[5]);
		boundsList->push_back(CBoundingBox(min, max));
	}

	return boundsList;
}

vector<EnemyInfo>* FileReader::ReadEnemies()
{
	ifstream infile("Assets/Enemies.Zac");
	string line;
	string d = ", "; //Delimiter

	float enemyInfo[5];
	//Schema:
	//Xpos
	//YPos
	//Zpos
	//YRot
	//eAction

	while (getline(infile, line))
	{
		string s = line;
		string delimiter = ", ";

		size_t pos = 0;
		string token;
		string::size_type sz; //keep stof happy
		int i = 0;
		while ((pos = s.find(delimiter)) != std::string::npos)
		{
			token = s.substr(0, pos);
			enemyInfo[i] = stof(token, &sz);
			s.erase(0, pos + delimiter.length());
			i++;
		}

		EnemyInfo e;
		e.Xpos = enemyInfo[0];
		e.Ypos = enemyInfo[1];
		e.Zpos = enemyInfo[2];

		e.YRot = enemyInfo[3];

		int a = enemyInfo[4];
		e.action = static_cast<Enemy::eAction>(a);

		enemyInfoList->push_back(e);
	}

	return enemyInfoList;
}
