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
// File			: helloevent.cpp
// Module		: helloevent
// Description	: helloevent (example application)
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
// XAA	040416	Created file 	
//
//*****************************************************************************

// Includes
#include "gzBaseLibrary.h"
#include "gzDistLibrary.h"


// Common constants
static const gzString HELLO_SESSION = "hello";
static const gzString HELLO_ATTRIBUTE = "hello";


//------------------------------ EventSender ----------------------------------

// A client that send events
class EventSender : public gzDistClientInterface
{
	public:

		EventSender();

		virtual ~EventSender();


	private:

		// thread ticker callback

		gzVoid onClientTick();
	
	private:

		// members

		gzDistSessionPtr m_session;

		gzUInt32 m_counter;

};


EventSender::EventSender() : gzDistClientInterface("sender"), m_counter(0)
{
	// Initialize client with tick interval approx 1 sec
	initialize(1);

	// Get a local session (create it if it does not exist)
	m_session = getSession(HELLO_SESSION, TRUE);

	// Join the session
	joinSession(m_session);

}


EventSender::~EventSender()
{
	// Uninitialize client
	uninitialize();
}


gzVoid EventSender::onClientTick()
{
	// Create an event
	gzDistEventPtr event = new gzDistEvent;

	// Add an attribute
	event->setAttributeValue(HELLO_ATTRIBUTE, gzString("Hello ")+gzString(m_counter));

	// Send the event
	sendEvent(event, m_session);

	++m_counter;
	
}

//------------------------------ EventReceiver ----------------------------------

// A client that receive events
class EventReceiver : public gzDistClientInterface
{
	public:

		EventReceiver();

		virtual ~EventReceiver();


	private:

		// notification callbacks

		gzVoid onEvent(gzDistEvent* event);


	private:

		// members

		gzDistSessionPtr m_session;

};


EventReceiver::EventReceiver() : gzDistClientInterface("receiver")
{
	// Initialize client (no ticker)
	initialize();

	// Get a local session (create it if it does not exist)
	m_session = getSession(HELLO_SESSION, TRUE);

	// Join the session
	joinSession(m_session);

	// Subscribe for events
	subscribeEvents(gzDistEvent::getClassType(), m_session);
}


EventReceiver::~EventReceiver()
{
	// Uninitialize client
	uninitialize();
}


gzVoid EventReceiver::onEvent(gzDistEvent* event)
{
	gzString attributeValue = event->getAttributeString(HELLO_ATTRIBUTE);

	GZMESSAGE(GZ_MESSAGE_NOTICE, attributeValue);
}


//--------------------------------- main --------------------------------------

// Application entry point
int main(int argc, char* argv[])
{
	// Setup default message receiver
	gzMessage::setMessageLevel(GZ_MESSAGE_NOTICE);

	// Create manager and start
	gzDistManagerPtr manager = gzDistManager::getManager(TRUE);
	manager->start();

	// Create receiver client
	EventReceiver* receiver = new EventReceiver;

	// Create sender client
	EventSender* sender = new EventSender;


	// Run a while
	gzSleep(10 * GZ_SLEEP_SECOND);
	

	// Delete the sender
	delete sender;

	// Delete the receiver
	delete receiver;

	// Shut down and release manager
	manager->shutDown();
	manager = NULL;

	gzSleep(GZ_SLEEP_SECOND);

	return 0;

}

