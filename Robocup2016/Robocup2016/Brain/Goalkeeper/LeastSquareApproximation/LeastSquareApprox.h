/*
 * LeastSquareApprox.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_SQUAREAPPROX_H_
#define BRAIN_GOALKEEPER_SQUAREAPPROX_H_

#define NUM_OF_SAMPLES 7
#define MIN_NUM_OF_SAMPLES_TO_APPROX 4

#include "Includes.h"

class LeastSquareApprox
{
public:
	virtual double GetB(double t) = 0;
	virtual double GetT(double b) = 0;

protected:
	LeastSquareApprox();
	virtual ~LeastSquareApprox();

	static double SumVector(vector<double> vector);
	static double SumVectorPower2(vector<double> vector);
	static double SumMultiplyVectors(vector<double> tVector, vector<double> bVector);
	static Mat BuildDataMatrix(vector<double> inputVector, int dimensions);
};

#endif /* BRAIN_GOALKEEPER_SQUAREAPPROX_H_ */
