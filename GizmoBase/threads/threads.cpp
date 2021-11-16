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
// File			: threads.cpp
// Module		: 
// Description	: An example of running threads
// Author		: Anders Modén          
// Product		: Gizmo3D 2.11.48
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
// AMO  020102  Created file  
//
// ******************************************************************************
#include "gzThread.h"
#include "gzObject.h"

class MyThread : public gzThread , public gzNameInterface
{
public:
		
	MyThread(const gzString &name ) : gzNameInterface(name)
	{
		run();
	}
	
	virtual void process()
	{
		// Repeat as long as the controlling thread doesn't try to stop us
		while(!isStopping())
		{
			GZTRACE("Tick from %s",(const char *)getName());
			gzSleep(GZ_SLEEP_SECOND);
		}
	}
	
private:
	
};
	
int main(int argc , char *argv[] )
{
	gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

	MyThread thread1("Thread 1");

	gzSleep(GZ_SLEEP_SECOND/2);

	MyThread thread2("Thread 2");

	gzSleep(GZ_SLEEP_SECOND*10);

	thread1.stop(TRUE);
	thread2.stop(TRUE);

	return 0;
}
