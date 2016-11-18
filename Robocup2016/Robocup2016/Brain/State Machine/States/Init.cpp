#include "Init.h"
#include "../../BrainData.h"

Init::Init()
{
	m_StateName = State_Init;
	m_stateVariable = "Initializing...";
}

Init::~Init()
{

}

void Init::Run()
{
	BrainData::GetInstance();
}






