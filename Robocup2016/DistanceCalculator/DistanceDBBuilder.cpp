/*
 * DistanceDBBuilder.cpp
 *
 *  Created on: May 11, 2016
 *      Author: root
 */

#include "DistanceDBBuilder.h"

DistanceDBBuilder::DistanceDBBuilder()
{
	m_angleToZeroPixelDistance = new DatabaseWrapper(ZeroPixelsDistanceDatabasePath);
}

DistanceDBBuilder::~DistanceDBBuilder()
{
	delete m_angleToZeroPixelDistance;
}

void DistanceDBBuilder::CreateDatabaseForTilt(Mat whiteImage, int angle)
{
	int zeroPixelDistance = m_angleToZeroPixelDistance->GetValueFromKey(angle);
	map<int,int> database = CreateDatabse(whiteImage, zeroPixelDistance);

	string filePath = GetDatabaseFilePath(angle);
	WriteDatabaseToFile(database, filePath);
}

string DistanceDBBuilder::GetDatabaseFilePath(int angle)
{
	string databaseFolder = DistanceDatabaseFolderPath;
	string fileName = intToString(angle);
	return databaseFolder + fileName;
}

string DistanceDBBuilder::intToString(int number)
{
	ostringstream convert;  // stream used for the conversion
	convert << number; 		// insert the textual representation of 'Number' in the characters in the stream
	return convert.str();
}

map<int,int> DistanceDBBuilder::CreateDatabse(Mat whiteImage, int zeroPixelDistance)
{
	ofstream myfile("/home/robot/src/RobocupBIU/RoboCup2016/DistanceCalculator/log.txt");

	map<int, int> result;
	int middleColumn = 640 / 2;

	result[0] = zeroPixelDistance;

	int distance = zeroPixelDistance;
	int blockNumber = 0;
	int pixelsCount = 0;
	for (int row = FRAME_HEIGHT - 1;
			row > 0 && NUM_OF_BLOCKS > blockNumber && distance < 250;
			row--)
	{
		int currentPixel = (int) whiteImage.at<uchar>(row, middleColumn);
		int nextPixel = (int) whiteImage.at<uchar>(row - 1, middleColumn);

		myfile << "Row: " << FRAME_HEIGHT - row << ", Pixel: "
				<< (currentPixel == 255 ? "W" : "B") << endl;

		if (nextPixel != currentPixel)
		{
			if (blockNumber < 20)
			{
				distance += 5;
			}
			else
			{
				distance +=10;
			}
			result[pixelsCount] = distance;
			blockNumber++;
		}

		pixelsCount++;
	}

	for (map<int, int>::const_iterator it = result.begin(); it != result.end();
			++it) {
		myfile << "Distance: " << it->first << ", Pixels: " << it->second
				<< endl;
	}

	myfile.close();

	return result;
}

void DistanceDBBuilder::WriteDatabaseToFile(map<int,int> database, string filePath)
{
	ofstream out(filePath.c_str());

	typedef map<int,int>::iterator mapIterator;
	for(mapIterator iterator = database.begin(); iterator != database.end(); iterator++)
	{
		out << iterator->first << "," << iterator->second << endl;
	}

	out.close();
}
