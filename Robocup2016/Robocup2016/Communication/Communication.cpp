/*
 * Communication.cpp
 *
 *  Created on: May 26, 2016
 *      Author: laptopyellow
 */
#include "Communication.h"

Communication::Communication(){
	mUdpListener = UdpListener::GetInstance();
	//m_udp_sender = UdpSender::GetInstance();
}

Communication* Communication::mInstance = NULL;
Communication* Communication::GetInstance()
{
    if(mInstance == NULL)
    {
    	mInstance = new Communication();
        return mInstance;
    }
    else
    {
        return mInstance;
    }
}

Communication::~Communication()
{
	Communication::mInstance = NULL;
}

RoboCupGameControlData Communication::ReadDataAndClearFlag(){
	RoboCupGameControlData returnData = mUdpListener->GetData().SafeRead();
	mUdpListener->GetIsChangedFlag().SafeWrite(false);
	return returnData;
}

bool Communication::getFlag(){
	return mUdpListener->GetIsChangedFlag().SafeRead();
}


