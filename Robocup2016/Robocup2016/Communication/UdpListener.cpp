/*
 * UdpListener.cpp
 *
 *  Created on: May 26, 2016
 *      Author: laptopyellow
 */
#include "UdpListener.h"

UdpListener* UdpListener::m_instance = NULL;

UdpListener* UdpListener:: GetInstance(){
	if(m_instance == NULL)
	{
		m_instance = new UdpListener();
		return m_instance;
	}
	else
	{
		return m_instance;
	}
}

void inline UdpListener::stopThread()
		{
			m_stop=true;
			m_listen_thread.join();
		}

UdpListener::UdpListener():	 m_port_number("3838"),
							 m_desired_version(8),
							 m_addr_len(sizeof(m_their_addr)),
							 m_stop(false),
							 m_listen_thread(std::thread(&UdpListener::Listen,this))
	{
	m_header[HEADER_LENGTH]='\0';
	mIsDataChanged.SafeWrite(false);

    }

UdpListener::~UdpListener(){
	if (m_instance !=NULL){
			delete m_instance;
			close(m_socket_fd);
		}
}

void UdpListener::Init(){

m_socket_fd = UdpUtils::CreateUdpReceiveSocket(NULL,m_port_number);
}


int UdpListener::Receive(){
	int numbytes;
	if ((numbytes = recvfrom(m_socket_fd,(void*)m_byte_buffer, BUFFER_LENGTH-1 , 0,
			        (sockaddr *)&m_their_addr, &m_addr_len)) == -1) {
			        throw CommunicationException("recvfrom");
			    }
	return numbytes;
}


HeaderType UdpListener::GetHeader(){
	strncpy(m_header,m_byte_buffer,HEADER_LENGTH);
	return ParseHeader();
}

HeaderType UdpListener::ParseHeader(){
	if (strcmp(m_header,"RGme")==0) return RGme;
	return unknown;
}

bool UdpListener::is_data_changed(RoboCupGameControlData* old_data, RoboCupGameControlData* new_data){
		//checking whether interesting fields have been changed
		if (old_data->state != new_data->state)	return true;
		if (old_data->firstHalf != new_data->firstHalf)	return true;
		if (old_data->secondaryState != new_data->secondaryState)	return true;
		if (old_data->teams[0].teamNumber == TEAM_NUMBER){
			if (old_data->teams[0].players[PLAYER_NUMBER].penalty 			  != new_data->teams[0].players[PLAYER_NUMBER].penalty)	return true;
			if (old_data->teams[0].players[PLAYER_NUMBER].secsTillUnpenalised != new_data->teams[0].players[PLAYER_NUMBER].secsTillUnpenalised)	return true;
		}
		else{
			if (old_data->teams[1].players[PLAYER_NUMBER].penalty 			  != new_data->teams[1].players[PLAYER_NUMBER].penalty)	return true;
			if (old_data->teams[1].players[PLAYER_NUMBER].secsTillUnpenalised != new_data->teams[1].players[PLAYER_NUMBER].secsTillUnpenalised)	return true;
		}
		return false;

}


void UdpListener::Listen(){
	Init();
	cout<<"start listening"<<endl;
	RoboCupGameControlData oldData,newData;
	while(!m_stop){
		//cout << "before receive"<<endl;
		Receive();
		//cout << "received " <<m_byte_buffer<<endl;
		HeaderType type = GetHeader();
		switch(type){
		case RGme:{
				  uint8_t version;
				  memcpy(&version,m_byte_buffer+4,sizeof(version));
				  if (version ==m_desired_version){
					  oldData = newData;
					  memcpy(&newData,m_byte_buffer,sizeof(RoboCupGameControlData));
					  printf("penalty team0 %d , team1 %d  \n",newData.teams[0].players[PLAYER_NUMBER].penalty,newData.teams[1].players[PLAYER_NUMBER].penalty);
					  if (is_data_changed(&oldData, &newData)){
						  cout << "Data changed!! " << endl;
						  WriteGameData(newData);
						  RaiseDataChanged();
					  }
					  RoboCupGameControlReturnData returnData;
					  returnData.player = 1;
					  returnData.team = 2;
					  returnData.message = 0;
					  char serilized_data[sizeof (RoboCupGameControlReturnData)];
					  memcpy(serilized_data,&returnData,sizeof(returnData));
					  UdpSender::GetInstance()->Send(serilized_data);
				  }
				  break;
				}
		case unknown: {cout <<"unknown"<<endl;cout.flush();break;}
		}
	}
}

SharedMemory<RoboCupGameControlData>& UdpListener::GetData(){
	return mData;
}

SharedMemory<bool>& UdpListener::GetIsChangedFlag(){
	return mIsDataChanged;
}


void UdpListener::RaiseDataChanged(){
	bool raise = true;
	mIsDataChanged.SafeWrite(raise);
}

inline void UdpListener::WriteGameData(const RoboCupGameControlData& data){
	mData.SafeWrite(data);
}



