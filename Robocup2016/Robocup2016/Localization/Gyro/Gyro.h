/*
 * Gyro.h
 *
 *  Created on: Apr 14, 2016
 *      Author: root
 */

#ifndef LOCALIZATION_GYRO_GYRO_H_
#define LOCALIZATION_GYRO_GYRO_H_

#include "Includes.h"

class Gyro {
public:
	void RunThread();

	Gyro();
	virtual ~Gyro();
};

#endif /* LOCALIZATION_GYRO_GYRO_H_ */
