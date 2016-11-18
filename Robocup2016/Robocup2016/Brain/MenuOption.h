/*
 * MenuOption.h
 *
 *  Created on: Apr 5, 2016
 *      Author: root
 */

#ifndef BRAIN_MENUOPTION_H_
#define BRAIN_MENUOPTION_H_

#include "Includes.h"

enum MenuOption
{
	Play,
	Kick,
	Stand,
	Walk,
	TurnRight,
	TurnLeft,
	StopWalking,
	Reset,
	GetTilt,
	SetTilt,
	Run,
	Exit
};

static const string MenuOptionString[] = { "Play", "Kick", "Stand", "Walk",
		"TurnRight", "TurnLeft", "StopWalking", "Reset", "GetTilt", "SetTilt","Run",
		"Exit" };



#endif /* BRAIN_MENUOPTION_H_ */
