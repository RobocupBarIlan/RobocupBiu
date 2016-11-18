/*
 * Compass.cpp
 *
 *  Created on: Jun 28, 2016
 *      Author: root
 */

#include "Compass.h"

Compass* Compass::m_instance=NULL;

Robot::LinuxUM7 linux_um7( "/dev/compass" );
Robot::UM7 um7( &linux_um7 );

Compass::Compass() {
	if ( um7.Connect() == false )
	{
			printf( "Fail to connect UM7!\n" );
	}
	else
	{
			printf( "Success! UM7 connected.\n" );
	}
	if(UpdateAngle())
		m_initialAngle = um7.yaw;
	else {
		printf("Failed to initialize angle\n");
	}
}

Compass::~Compass() {
	// TODO Auto-generated destructor stub
}

Compass* Compass::GetInstance(){
	if(m_instance == NULL)
	{
		m_instance = new Compass();
		return m_instance;
	}
	else
	{
		return m_instance;
	}
}

double Compass::AlignAngleToMinus179toPlus180(const double &  angle){
	double temp;
	if (angle >= 0){
		temp = fmod(angle,360);
		if (temp>180)		temp = temp-360;
	}
	else{	//negative angle
		temp = fmod(angle,360);
		if (temp < (-179))	temp = temp+360;
	}
	return temp;
}

bool Compass::UpdateAngle(){
	return um7.request_angles();
}

# define ANGLE_NOT_VALID	300
double Compass::GetCurrentAngle(){
	//return AlignAngleToMinus179toPlus180(um7.yaw - m_initialAngle);
  	if (UpdateAngle())	return AlignAngleToMinus179toPlus180(um7.yaw - m_initialAngle);
	else 				return ANGLE_NOT_VALID;

}
