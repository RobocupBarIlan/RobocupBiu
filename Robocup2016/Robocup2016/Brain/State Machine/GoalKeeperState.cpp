/*
 * GoalKeeperState.cpp
 *
 *  Created on: Jun 22, 2016
 *      Author: root
 */

#include "GoalKeeperState.h"

GoalKeeperState::GoalKeeperState() {
	Logger = Log::GetInstance();
	m_Motion = Motion::GetInstance();
	m_Vision = Vision::GetInstance();
}

GoalKeeperState::~GoalKeeperState() {
	//delete Logger;
}

string GoalKeeperState::NameToString()
{
	return GKStateNamesStrings[m_StateName];
}

GoalKeeperStateName GoalKeeperState::Name()
{
	return m_GKStateName;
}

string GoalKeeperState::GetStateVariable()
{
	return m_GKStateVariable;
}

void GoalKeeperState::WriteLine(string message)
{
	//static const string StatesStrings[] = { "Init", "GoForBall" };
	string stateName = NameToString();//StatesStrings[m_StateName];
	Logger->Info(message + "\r\n", stateName);
}
