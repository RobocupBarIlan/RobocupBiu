/*
 * DatabaseWrapper.h
 *
 *  Created on: May 11, 2016
 *      Author: root
 */

#ifndef DATABASEWRAPPER_H_
#define DATABASEWRAPPER_H_

#include "Includes.h"

class DatabaseWrapper
{
public:
	DatabaseWrapper(string filePath);
	virtual ~DatabaseWrapper();

	int GetValueFromKey(int key);

	static vector<int> GetMapKeys(map<int, int> inputMap);
	static vector<int> GetMapValues(map<int, int> inputMap);

private:
	map<int, int> m_DatabaseMap;

	map<int,int> LoadMapFromFile(string filePath);
	vector<string> StringSplit(const string& str, const string& delimiter);

	int FindClosestKey(int key);
};

#endif /* DATABASEWRAPPER_H_ */
