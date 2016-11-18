#include "State.h"

State::State()
{
	Logger = Log::GetInstance();
	m_Motion = Motion::GetInstance();
	m_Vision = Vision::GetInstance();
}

State::~State()
{
	delete Logger;
}

string State::NameToString()
{
	return StateNamesStrings[m_StateName];
}

StateName State::Name()
{
	return m_StateName;
}

string State::GetStateVariable()
{
	return m_stateVariable;
}


void State::WriteLine(string message)
{
	//static const string StatesStrings[] = { "Init", "GoForBall" };
	string stateName = NameToString();//StatesStrings[m_StateName];
	Logger->Info(message + "\r\n", stateName);
}
