/*
 * LineApprox.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_LINEAPPROX_H_
#define BRAIN_GOALKEEPER_LINEAPPROX_H_

#include "LeastSquareApprox.h"

// Approx for line. b = C + Dt.
class LineApprox : public LeastSquareApprox
{
public:
	LineApprox(double C, double D);
	virtual ~LineApprox();

	double GetB(double t);
	double GetT(double b);

	static LeastSquareApprox* Approx(vector<double> tVector, vector<double> bVector);

protected:
	double m_C;
	double m_D;
};

#endif /* BRAIN_GOALKEEPER_LINEAPPROX_H_ */
