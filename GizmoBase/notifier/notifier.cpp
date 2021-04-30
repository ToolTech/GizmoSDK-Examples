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
// File			: notifier.cpp
// Module		: 
// Description	: An example of prioritized notifications
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.1
//              
//
//                      
// NOTE:	The GIZMO package defines a general purpose API for large model
//			visualisation and advanced graphic behaviours. The package is similar
//			to Cosmo3D, Inventor, Performer etc. but adds automated behaviour
//			patterns to the graphics. 
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// AMO  991002  Created file  
//
// ******************************************************************************
#include "gzThread.h"
#include "gzNotify.h"
#include "gzObject.h"

gzNotifyManager systemManager;

class subscriber : public gzNotifyInterface
{
public:
		
	subscriber(gzString name)
	{
		m_name=name;
	}
	
	virtual gzVoid onNotify(gzNotifyManager *master,gzRefPointer<gzNotifyMessage> &message,gzUInt32 senderClass)
	{
		GZTRACE("%s:Received %s\n",(const char *)m_name,(const char *)message->getCommand());
	}
	
	virtual ~subscriber()
	{
	}
	
private:
		
	gzString m_name;
};



class injector : public gzThread , public gzTypeInterface
{
public:
		
	GZ_DECLARE_TYPE_INTERFACE;

	injector(gzString name="injector" , gzUInt32 priority=1, gzUInt32 sleep=1)
	{
		m_name=name;
		m_priority=priority;
		m_sleep=sleep;
		
		run();
	}
	
	virtual void process()
	{
		while(!isStopping())
		{
			systemManager.notify(m_name,m_priority,500,this,NULL,0);
			gzSleep(m_sleep);
		}
	}
	
private:
		
	gzUInt32         m_priority;
	gzUInt32         m_sleep;
	gzString        m_name;
		
};
	
GZ_DECLARE_TYPE_BASE(injector,"injector");
	

int main(int argc , char *argv[] )
{
	gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	injector high("high:3",3,40);
	injector med("med:2",2,40);
	injector low("low:1",1,40);
	
	for(int i=0;i<10;i++)
	{
		subscriber *sub=new subscriber(gzString("sub:")+gzString(i));
		systemManager.addSubscriber(sub);
	}
		
	gzInt32 counter=1000;

	while(counter--)
	{
		systemManager.triggerNotifications();
		gzSleep(1);
	}

	return 1;
}
