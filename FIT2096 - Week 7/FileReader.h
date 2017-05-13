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

class FileReader
{
public:
	FileReader();
	~FileReader();
	vector<CBoundingBox>* Read();
private:
	vector<CBoundingBox>* boundsList = new vector<CBoundingBox>;
};



FileReader::FileReader()
{
	
}

FileReader::~FileReader()
{
	boundsList->clear();
}

inline vector<CBoundingBox>* FileReader::Read()
{
	ifstream infile("C:/Users/Dook/Desktop/LevelBounds.Zac");
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
