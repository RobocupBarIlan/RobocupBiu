/*
 * ParabolaApprox.h
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#ifndef BRAIN_GOALKEEPER_PARABOLAAPPROX_H_
#define BRAIN_GOALKEEPER_PARABOLAAPPROX_H_

#include "LineApprox.h"

// Approx for parabola. b = C + Dt + Et^2.
class ParabolaApprox : public LineApprox
{
public:
	ParabolaApprox(double C, double D, double E);
	virtual ~ParabolaApprox();

	double GetB(double t);
	double GetT(double b);

	static LeastSquareApprox* Approx(vector<double> tVector, vector<double> bVector);

private:
	double m_E;
};

#endif /* BRAIN_GOALKEEPER_PARABOLAAPPROX_H_ */
