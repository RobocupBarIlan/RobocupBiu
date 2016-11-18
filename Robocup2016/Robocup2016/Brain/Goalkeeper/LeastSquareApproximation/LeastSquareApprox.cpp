/*
 * LineApprox.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "LeastSquareApprox.h"

LeastSquareApprox::LeastSquareApprox()
{

}

LeastSquareApprox::~LeastSquareApprox()
{

}

double LeastSquareApprox::SumVector(vector<double> vector)
{
	double result = 0;
	for (double i : vector)
	{
		result += i;
	}
	return result;
}

double LeastSquareApprox::SumVectorPower2(vector<double> vector)
{
	double result = 0;
	for (double i : vector)
	{
		result += pow(i,2);
	}
	return result;
}

double LeastSquareApprox::SumMultiplyVectors(vector<double> v1, vector<double> v2)
{
	double result = 0;
	for (int i = 0 ; i < v1.size() ; i++)
	{
		result += v1[i] * v2[i];
	}
	return result;
}

Mat LeastSquareApprox::BuildDataMatrix(vector<double> inputVector, int dimensions)
{
	vector<double> dataAsVector;

		for (int i = 0 ; i < inputVector.size() ; i++)
		{
			for (int j = 0 ; j <= dimensions ; j++)
			{
				dataAsVector.push_back(pow(inputVector[i],j));
			}
		}

		Mat A(inputVector.size(), dimensions + 1, CV_64FC1);
		memcpy(A.data,dataAsVector.data(),dataAsVector.size()*sizeof(double));

		return A;
}
