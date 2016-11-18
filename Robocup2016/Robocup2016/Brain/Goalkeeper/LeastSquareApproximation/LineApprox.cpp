/*
 * LineApprox.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "LineApprox.h"

LineApprox::LineApprox(double C, double D)
	: m_C(C), m_D(D)
{
}

LineApprox::~LineApprox() {
	// TODO Auto-generated destructor stub
}

double LineApprox::GetB(double t)
{
	return m_C + m_D * t;
}

double LineApprox::GetT(double b)
{
	return (b - m_C) / m_D;
}

LeastSquareApprox* LineApprox::Approx(vector<double> tVector, vector<double> bVector)
{
	double sumTi = SumVector(tVector);
	double sumTi2 = SumVectorPower2(tVector);
	double sumBi = SumVector(bVector);
	double sumTiBi = SumMultiplyVectors(tVector, bVector);

	double AAtData[4] = { tVector.size(), sumTi, sumTi, sumTi2 };
	Mat AAt(2, 2, CV_64FC1, AAtData);
	//cout << "AAt: " << AAt << endl;

	double AtBData[2] = { sumBi, sumTiBi };
	Mat AtB(2, 1, CV_64FC1, AtBData);
	//cout << "AtB: " << AtB << endl;

	Mat AAtInv = AAt.inv();
	//cout << "AAt inverse: " << AAtInv << endl;

	Mat CD = AAtInv * AtB;
	//cout << "CD: " << CD << endl;

	return new LineApprox(CD.at<double>(0), CD.at<double>(1));
}
