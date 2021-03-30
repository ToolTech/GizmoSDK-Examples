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
// File			: ownership.cpp
// Module		: ownership
// Description	: ownership (example application)
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
// XAA	040421	Created file 	
//
//*****************************************************************************

// Includes
#include <stdio.h>
#include "gzBaseLibrary.h"
#include "gzDistLibrary.h"


// Common constants
static const gzString EXAMPLE_SESSION = "EXAMPLE_SESSION";
static const gzString EXAMPLE_ATTRIBUTE = "EXAMPLE_ATTRIB";
static const gzString NAME_A = "A";
static const gzString NAME_B = "B";
static const gzString CLIENT_PREFIX = "CLIENT_";
static const gzString OBJECT_PREFIX = "OBJECT_";


//-----------------------------------------------------------------------------

// Example owner client
class Client : public gzDistClientInterface
{
	public:

		Client(const gzString& name);

		virtual ~Client();


	private:

		// callbacks

		gzVoid onClientTick();

		gzVoid onNewObject(gzDistObject* object, gzDistSession* session);

		gzVoid onRemoveObject(gzDistObject* object, gzDistSession* session);

		gzVoid onUpdateAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session);

		gzVoid onGrantOwnership(const gzString& attributeName, gzDistObject* object, gzDistSession* session);

		gzVoid onDropOwnership(const gzString& attributeName, gzDistObject* object, gzDistSession* session);

		gzVoid onRequestOwnership(const gzString& attributeName, gzDistObject* object, const gzDistClientID& requester, gzDistSession* session);

		gzVoid onSystemShutdown();


	private:

		// members

		gzDistSessionPtr m_session;

		gzRefList<gzDistObject> m_objects;

		gzUInt32 m_tickCount;

};


Client::Client(const gzString& name)
 :	gzDistClientInterface(CLIENT_PREFIX + name),
	m_tickCount(0)
{
	// initialize client
	initialize(2);

	// create and join session
	m_session = getSession(EXAMPLE_SESSION, TRUE);
	joinSession(m_session);

	// subscribe for objects
	subscribeObjects(gzDistObject::getClassType(), m_session, TRUE);

	// add an object using my own name
	gzDistObjectPtr object = new gzDistObject(OBJECT_PREFIX + name);
	object->setAttributeValue(EXAMPLE_ATTRIBUTE, GZ_EMPTY_STRING);
	addObject(object, m_session);

}


Client::~Client()
{
	// unsubscribe objects
	unsubscribeObjects(gzDistObject::getClassType(), m_session);

	// resign the session
	resignSession(m_session);

	// uninitialize client
	uninitialize();
}


gzVoid Client::onClientTick()
{
	gzListIterator<gzDistObject> iter(m_objects);
	gzDistObject* object;
	
	while (object = iter())
	{
		if (!updateObject(EXAMPLE_ATTRIBUTE, getID().name + gzString(m_tickCount), object, GZ_DIST_SYNCHRONOUS))
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "'%s' failed to update '%s' on '%s'",
						(const char*)getID().name, (const char*)EXAMPLE_ATTRIBUTE, (const char*)object->getName());
		}

		if (!object->isOwnerOf(EXAMPLE_ATTRIBUTE, getID()))
		{
			requestOwnership(EXAMPLE_ATTRIBUTE, object);
		}

	}

	++m_tickCount;

}


gzVoid Client::onNewObject(gzDistObject* object, gzDistSession* session)
{
	m_objects.insert(object);

	if (object->getName() == getID().name)
	{
		requestOwnership(EXAMPLE_ATTRIBUTE, object);
		subscribeAttributeValue(EXAMPLE_ATTRIBUTE, object);
	}
}


gzVoid Client::onRemoveObject(gzDistObject* object, gzDistSession* session)
{
	m_objects.remove(object);
}


gzVoid Client::onUpdateAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session)
{
	gzDistNotificationSetIterator iter(attributes);
	gzDistNotificationData* data;

	while (data = iter())
	{
		GZMESSAGE(GZ_MESSAGE_NOTICE, "'%s' on '%s' was updated to '%s'",
					(const char*)data->getAttributeName(), (const char*)object->getName(),
					(const char*)data->getAttributeValue().getString());
	}
	
}

		
gzVoid Client::onGrantOwnership(const gzString& attributeName, gzDistObject* object, gzDistSession* session)
{
	// ownership was granted
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Ownership of '%s' on '%s' was granted to '%s'",
				(const char*)attributeName, (const char*)object->getName(), (const char*)getID().name);
}


gzVoid Client::onDropOwnership(const gzString& attributeName, gzDistObject* object, gzDistSession* session)
{
	// ownership was dropped
	GZMESSAGE(GZ_MESSAGE_NOTICE, "'%s' dropped ownership of '%s' on '%s'",
				(const char*)getID().name, (const char*)attributeName, (const char*)object->getName());
}


gzVoid Client::onRequestOwnership(const gzString& attributeName, gzDistObject* object, const gzDistClientID& requester, gzDistSession* session)
{
	// someone has requested ownership of my attribute
	GZMESSAGE(GZ_MESSAGE_NOTICE, "'%s' requested ownership of '%s' on '%s'",
				(const char*)requester.name, (const char*)attributeName, (const char*)object->getName());
}


gzVoid Client::onSystemShutdown()
{
	// in case of pre-mature shutdown
	m_objects.clear();
	m_session = NULL;	
}


//-----------------------------------------------------------------------------

// Application entry point
int main(int argc, char* argv[])
{
	// setup default message receiver
	gzMessage::setMessageLevel(GZ_MESSAGE_NOTICE);

	// create manager and start
	gzDistManagerPtr manager = gzDistManager::getManager(TRUE);
	manager->start();

	GZMESSAGE(GZ_MESSAGE_NOTICE, "Hit return to quit!\n");
	
	// create clients
	gzDistClientInterface* clientA = new Client(NAME_A);
	gzDistClientInterface* clientB = new Client(NAME_B);


	// wait for return
	getchar();
	

	// delete clients
	delete clientA;
	delete clientB;

	// shut down and release manager
	manager->shutDown();
	manager = NULL;

	gzSleep(GZ_SLEEP_SECOND);

	return 0;

}

