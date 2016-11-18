/*
 * Compass.h
 *
 *  Created on: Jun 28, 2016
 *      Author: root
 */

#ifndef BRAIN_COMPASS_H_
#define BRAIN_COMPASS_H_

#include "../../../../HROS5-Framework/Linux/include/LinuxUM7.h"
#include "../../../../HROS5-Framework/Framework/include/UM7.h"
#include <math.h>

class Compass {
public:
	static Compass* GetInstance();
	double GetCurrentAngle();
	virtual ~Compass();
private:
	double m_initialAngle;
	Compass();
	static Compass* m_instance;
	double AlignAngleToMinus179toPlus180(const double&  angle);
	bool UpdateAngle();
};

#endif /* BRAIN_COMPASS_H_ */
