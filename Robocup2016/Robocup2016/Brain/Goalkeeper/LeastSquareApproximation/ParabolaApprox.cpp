/*
 * ParabolaApprox.cpp
 *
 *  Created on: Jun 20, 2016
 *      Author: root
 */

#include "ParabolaApprox.h"

ParabolaApprox::ParabolaApprox(double C, double D, double E)
	: LineApprox(C, D), m_E(E)
{
}

ParabolaApprox::~ParabolaApprox() {
	// TODO Auto-generated destructor stub
}

double ParabolaApprox::GetB(double t)
{
	return m_C + m_D * t + m_E * pow(t,2);
}

double ParabolaApprox::GetT(double b)
{
	return (-m_D + sqrt(pow(m_D,2) - 4 * (m_C - b) * m_E)) / 2 * m_E;
}

LeastSquareApprox* ParabolaApprox::Approx(vector<double> tVector, vector<double> bVector)
{
	Mat A = BuildDataMatrix(tVector, 2);
	//cout << A << endl;
	Mat ATran;
	transpose(A, ATran);

	Mat AAtInv = (ATran * A).inv();
	Mat bMat(tVector.size(), 1, CV_64FC1);
	memcpy(bMat.data,bVector.data(),bVector.size()*sizeof(double));
	//cout << "B: " << bMat << endl;

	Mat AtB = ATran * bMat;
	Mat CDE = AAtInv * AtB;
	//cout << "CDE: " << CDE << endl;

	//cout << "Approx: " <<  A*CDE << endl;

	return new ParabolaApprox(CDE.at<double>(0), CDE.at<double>(1), CDE.at<double>(2));
}
