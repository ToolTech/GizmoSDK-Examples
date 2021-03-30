//*****************************************************************************
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
// File			: main.cpp
// Module		: main
// Description	: main (example application)
// Author		: Anders Sandblad		
// Product		: GizmoDistribution
//		
//
//			
// NOTE:	GizmoDistribution is a toolkit used for implementing distributed 
//			objects and events in C++
//
//
// Revision History...							
//									
// Who	Date	Description						
//									
// XAA	040423	Created file 	
//
//*****************************************************************************

// Includes
#include "gzBaseLibrary.h"
#include "gzDistLibrary.h"

// Use this define to enable global distribution
//#define GLOBAL


// Application entry point
int main(int argc, char* argv[])
{
	// Setup default message receiver
	gzMessage::setMessageLevel(GZ_MESSAGE_NOTICE);

#ifndef GLOBAL

	// Create the manager and start local distribution
	gzDistManagerPtr manager = gzDistManager::getManager(TRUE);
	manager->start();

	GZMESSAGE(GZ_MESSAGE_NOTICE, "Hello Local GizmoDistribution !!!");

#else

	// To enable global distribution you should
	// provide two channels (with different transports).
	// One channel is used for data distribution and
	// the other is used for server control messages.

	// Create the manager
	gzDistManagerPtr manager = gzDistManager::getManager(TRUE);

	// This is the simplest way to do it. It uses default parameters.
	// See GizmoDistribution online documentation for details.
	manager->start(gzDistCreateDefaultSessionChannel(), gzDistCreateDefaultServerChannel());

/*	*** Note: If you want to specify IP and ports, do as below: ***

	// Setup session channel
	gzDistRemoteChannel* sessionChannel = new gzDistRemoteChannel;
	gzDistTransportUDP* sessionTransport = new gzDistTransportUDP;

!	// For multicast...																\
!	sessionTransport->createMulticast(gzSocketAddress(gzHostAddress(234,56,78,90), 2345));	 |
!																							 |
!	// ...or for broadcast																	 | NOTE: Use only one of these!
!	sessionTransport->createBroadcast(2345);												 |
																							/
	sessionChannel->setTransport(sessionTransport);
	
	// Setup server channel
	gzDistRemoteChannel* serverChannel = new gzDistRemoteChannel;
	gzDistTransportUDP* serverTransport = new gzDistTransportUDP;

!	// For multicast...																\
!	serverTransport->createMulticast(gzSocketAddress(gzHostAddress(234,56,78,90), 5432));	 |
!																							 |
!	// ...or for broadcast:																	 | NOTE: Use only one of these!
!	serverTransport->createBroadcast(5432);													 |
																							/
	serverChannel->setTransport(serverTransport);

	// Start manager for global distribution
	manager->start(sessionChannel, serverChannel);
*/
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Hello Global GizmoDistribution !!!");

#endif

	// run a while
	gzSleep(2 * GZ_SLEEP_SECOND);

	// shut down and release manager
	manager->shutDown();
	manager = NULL;

	gzSleep(GZ_SLEEP_SECOND);

	return 0;

}

