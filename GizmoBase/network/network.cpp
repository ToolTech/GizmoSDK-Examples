// *****************************************************************************
//
// Copyright (C) SAAB AB
//
// All rights, including the copyright, to the computer program(s)
// herein belong to Saab AB. The program(s) may be used and/or
// copied only with the written permission of Saab AB, or in
// accordance with the terms and conditions stipulated in the
// agreement/contract under which the program(s) have been
// supplied.
//
//
// Information Class:	COMPANY UNCLASSIFIED
// Defence Secrecy:		NOT CLASSIFIED
// Export Control:		NOT EXPORT CONTROLLED
//
//
// File			: network.cpp
// Module		: 
// Description	: Test app for network communication
// Author		: Anders Modén          
// Product		: GizmoBase 2.11.1
//              
//
//                      
// NOTE:	Gizmo3D is a high performance 3D Scene Graph and effect visualisation 
//			C++ toolkit for Linux, Mac OS X, Windows (Win32) and IRIX® for  
//			usage in Game or VisSim development.
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// AMO  030420  Created file    
//
// ******************************************************************************

#include "gzBaseLibrary.h"
#include <iostream>

const gzDouble TIMEOUT=2.0;

class NetworkData : public gzReference
{
public:

	NetworkData()
	{
		lastReceiveTime=gzTime::systemSeconds();
		lastReceiveID=0;
	}

	virtual ~NetworkData(){};


	gzHostAddress	address;
	gzUInt32			port;
	gzDouble		lastReceiveTime;
	gzUInt32			lastReceiveID;
};

class MyBroadcastReceiver : public gzNotifyInterface , public gzThread , public gzMutex
{
public:
	MyBroadcastReceiver(gzBool multicast,gzUInt32 port,gzBool quiet):m_quiet(quiet)
	{

		m_socket.useExceptions(TRUE);

		if(multicast)
		{
			std::cout << "Multicast network receiver" << std::endl;
			m_socket.enableMulticast(gzHostAddress(224,56,78,90), port); // Multicast
		}
		else
		{
			std::cout << "Broadcast network receiver" << std::endl;
			m_socket.enableBroadcast(port);	// Broadcast
		}

		m_socket.setTickInterval(0.01);	// Check the slot every 0.01 second

		m_socket.addSubscriber(this);	// Add me as subsciber to my socket

		run();
	}

	gzVoid onNotify(gzNotifyManager *sender,gzRefPointer<gzNotifyMessage> &message,gzUInt32 senderClass)
	{
		gzNetData *data=(gzNetData *)message->getData();

		gzSocketAddress inet=data->sender;

		NetworkData *ndata;

		waitLock();

		gzListIterator<NetworkData> iterator(m_networkData);

		while(ndata=iterator())
		{
			if((ndata->address==inet.getHostAddress()) && (ndata->port==inet.getPort()))
				break;
		}

		if(!ndata)
		{
			ndata=new NetworkData;

			m_networkData.insert(ndata);

			ndata->address=inet.getHostAddress();

			ndata->port=inet.getPort();
		}

		gzUInt32 networkID=*(gzUInt32 *)data->data.getAddress();

		gzUInt32 id=gzSwapNetworkBytesL(networkID);

		if(ndata->lastReceiveID==0)
		{
			ndata->lastReceiveID=id-1;
			std::cout<<"DISCOVER "<<(const char *)inet.getHostAddress().asString()<<" at port "<<inet.getPort()<<std::endl;
		}

		if(ndata->lastReceiveID!=(id-1))
		{
			std::cout<<"missed data from "<<(const char *)inet.getHostAddress().asString()<<" at port "<<inet.getPort()<<" from index:"<<ndata->lastReceiveID+1<<" to index:"<<id-1<<std::endl;
			gzBeep();
		}

		if(ndata->lastReceiveTime < (gzTime::systemSeconds()-TIMEOUT))
		{
			std::cout<<"Timeout from "<<(const char *)inet.getHostAddress().asString()<<" at port "<<inet.getPort()<<" Diff:"<<gzTime::systemSeconds()-ndata->lastReceiveTime<<" seconds"<<std::endl;
			gzBeep();
		}

		ndata->lastReceiveID=id;
		ndata->lastReceiveTime=gzTime::systemSeconds();

		if(!m_quiet)
			std::cout<<"Data from "<<(const char *)inet.getHostAddress().asString()<<" at port "<<inet.getPort()<<" at time "<<(const char *)gzTime::now().asString()<<std::endl;

		unLock();
		
	}

	gzVoid virtual process()
	{
		while(TRUE)
		{
			waitLock();

			NetworkData *ndata;

			gzDouble time=gzTime::systemSeconds();

			gzListIterator<NetworkData> iterator(m_networkData);

			while(ndata=iterator())
			{
				if(ndata->lastReceiveTime < (time-TIMEOUT))
				{
					std::cout<<"Timeout from "<<(const char *)ndata->address.asString()<<" at port "<<ndata->port<<" Diff:"<<time-ndata->lastReceiveTime<<" seconds"<<std::endl;
					gzBeep();
				}
			}

			unLock();

			gzSleep(GZ_SLEEP_SECOND);
		}
	}

private:

	gzSocket				m_socket;

	gzRefList<NetworkData>	m_networkData;

	gzBool					m_quiet;

};


int main(int argc, char *argv[])
{
    gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	gzArgumentParser arg(argc,argv);

	arg.setSyntaxString("{-p portnumber : port to receive on} {-m : multicast} {-q : quiet mode}");

	gzBool useMulticast=arg.hasOption("m");

	gzBool quiet=arg.hasOption("q");

	gzUInt32 port=arg.getOptionValue("p",1234);

	try
	{
		MyBroadcastReceiver receiver(useMulticast,port,quiet);

		while(TRUE)
			gzSleep(GZ_SLEEP_SECOND);

	}
	catch(gzBaseError &error)       // In case of exceptions thrown we want to print the message
	{
		error.reportError();
		gzSleep(GZ_SLEEP_SECOND);
	}

	return 0;
}

