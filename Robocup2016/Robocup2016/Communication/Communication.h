#ifndef COMM_H
#define COMM_H

#include "UdpSender.h"
#include "UdpListener.h"


class Communication {
public:
	static Communication* GetInstance();
	~Communication();
	RoboCupGameControlData ReadDataAndClearFlag();
	bool getFlag();

private:
	UdpListener* mUdpListener;
	UdpSender* mUdpSender;
	static Communication* mInstance;
	Communication();

};

#endif
