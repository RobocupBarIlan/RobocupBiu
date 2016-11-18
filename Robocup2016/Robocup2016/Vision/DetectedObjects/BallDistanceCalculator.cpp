/*
 * BallDistanceCalculator.cpp
 *
 *  Created on: May 4, 2016
 *      Author: root
 */

#include "BallDistanceCalculator.h"

BallDistanceCalculator::BallDistanceCalculator()
{
	char* databaseFolderPath = DistanceDatabaseFolderPath;
	vector<string> files = FindFilesInFolder(databaseFolderPath);

	for (int i = 0 ; i < (int)files.size() ; i++)
	{
		string filePath = databaseFolderPath + files[i];
		int tiltKey = atoi(files[i].c_str());
		m_tiltToDatabase[tiltKey] = new DatabaseWrapper(filePath);
	}
}

BallDistanceCalculator::~BallDistanceCalculator()
{
	// TODO Auto-generated destructor stub
}

int BallDistanceCalculator::CalculateDistance(Point2f ballLocation, float radius, int headTilt)
{
	int largerTiltKey = FindClosestLargerTiltInMap(headTilt);
	int smallerTiltKey = FindClosestSmallerTiltInMap(headTilt);

	int largerDistance = m_tiltToDatabase[largerTiltKey]->GetValueFromKey(FRAME_HEIGHT - ((int)ballLocation.y + radius));
	int smallerDistance = m_tiltToDatabase[smallerTiltKey]->GetValueFromKey(FRAME_HEIGHT - ((int)ballLocation.y + radius));

	float actualKeyRatio = (float)(headTilt - smallerTiltKey) / (float)(largerTiltKey - smallerTiltKey);
	return smallerDistance + ((largerDistance - smallerDistance) * actualKeyRatio);
}

bool isNumber(const string& s)
{
	string::const_iterator it = s.begin();
	while (it != s.end() && (isdigit(*it) || *it == '-')) ++it;
	return !s.empty() && it == s.end();
}

int BallDistanceCalculator::FindClosestTiltInMap(int headTilt)
{
	vector<int> mapKeys;
	for (map<int, DatabaseWrapper*>::iterator it = m_tiltToDatabase.begin();
				it != m_tiltToDatabase.end(); ++it)
		{
			mapKeys.push_back(it->first);
		}

	int closestKey = mapKeys.front();
	for (int i = 0; i < (int) mapKeys.size(); i++)
	{
		if (abs(mapKeys[i] - headTilt) < abs(closestKey - headTilt))
		{
			closestKey = mapKeys[i];
		}
	}

	return closestKey;
}

int BallDistanceCalculator::FindClosestLargerTiltInMap(int headTilt)
{
	vector<int> mapKeys;
		for (map<int, DatabaseWrapper*>::iterator it = m_tiltToDatabase.begin();
					it != m_tiltToDatabase.end(); ++it)
			{
				mapKeys.push_back(it->first);
			}

		int closestKey = mapKeys.front();
		for (int i = 0; i < (int) mapKeys.size(); i++)
		{
			if (headTilt < mapKeys[i] && abs(mapKeys[i] - headTilt) < abs(closestKey - headTilt))
			{
				closestKey = mapKeys[i];
			}
		}

		return closestKey;
}

int BallDistanceCalculator::FindClosestSmallerTiltInMap(int headTilt)
{
	vector<int> mapKeys;
		for (map<int, DatabaseWrapper*>::iterator it = m_tiltToDatabase.begin();
					it != m_tiltToDatabase.end(); ++it)
			{
				mapKeys.push_back(it->first);
			}

		int closestKey = mapKeys.front();
		for (int i = 0; i < (int) mapKeys.size(); i++)
		{
			if (headTilt >= mapKeys[i] && abs(mapKeys[i] - headTilt) < abs(closestKey - headTilt))
			{
				closestKey = mapKeys[i];
			}
		}

		return closestKey;
}

vector<string> BallDistanceCalculator::FindFilesInFolder(const char* folderPath)
{
	vector<string> files;

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(folderPath)) != NULL)
	{
		int i = 0;
		while ((ent = readdir(dir)) != NULL)
		{
			string fileName = ent->d_name;
			if (isNumber(fileName))
			{
				files.push_back(fileName);
				i++;
			}
		}

		closedir(dir);
	}
	else
	{
		//Log::GetInstance()->Error("Couldn't find files in directory: " + folderPath, "DistanceCalc");
	}

	return files;
}

