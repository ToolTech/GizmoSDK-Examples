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
// File			: helloobject.cpp
// Module		: helloobject
// Description	: helloobject (example application)
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
static const gzString HELLO_OBJECT = "hello";
static const gzString HELLO_ATTRIBUTE = "hello";


//-----------------------------------------------------------------------------

// A client that publish objects
class ObjectPublisher : public gzDistClientInterface
{
	public:

		ObjectPublisher();

		virtual ~ObjectPublisher();


	private:

		// callbacks

		gzVoid onClientTick();

		gzVoid onNewObject(gzDistObject* object, gzDistSession* session);

		gzVoid onRemoveObject(gzDistObject* object, gzDistSession* session);

		gzVoid onSystemShutdown();


	private:

		// members

		gzDistSessionPtr m_session;

		gzDistObjectPtr m_object;

		gzUInt32 m_counter;

};


ObjectPublisher::ObjectPublisher() : gzDistClientInterface("publisher"), m_counter(0)
{
	// initialize client
	initialize(1);

	// get the session (create it if it does not exist)
	m_session = getSession(HELLO_SESSION, TRUE);

	// join the session
	joinSession(m_session);

	// subscribe for objects
	subscribeObjects(gzDistObject::getClassType(), m_session);

	// create an object
	gzDistObjectPtr object = new gzDistObject(HELLO_OBJECT);
	object->setAttributeValue(HELLO_ATTRIBUTE, gzString("Hello"));
	
	// add the object
	// this object instance is not necessarily identical to
	// the object actuallty added to the session
	// the added instance is captured in the onNewObject() callback
	addObject(object, m_session);

}


ObjectPublisher::~ObjectPublisher()
{
	// remove the object
	removeObject(m_object);

	// unsubscribe objects
	unsubscribeObjects(gzDistObject::getClassType(), m_session);

	// resign the session
	resignSession(m_session);

	// uninitialize client
	uninitialize();
}


gzVoid ObjectPublisher::onNewObject(gzDistObject* object, gzDistSession* session)
{
	if (!m_object)
	{
		m_object = object;
	}
}


gzVoid ObjectPublisher::onRemoveObject(gzDistObject* object, gzDistSession* session)
{
	if (object == m_object)
	{
		m_object = NULL;
	}
}


gzVoid ObjectPublisher::onClientTick()
{
	if (m_object)
	{
		updateObject(HELLO_ATTRIBUTE, gzString("Hello ")+gzString(m_counter), m_object);
		++m_counter;
	}
}


gzVoid ObjectPublisher::onSystemShutdown()
{
	// in case of pre-mature shutdown
	m_object = NULL;
	m_session = NULL;	
}


//-----------------------------------------------------------------------------

// A client that subscribe for objects and attributes
class ObjectSubscriber : public gzDistClientInterface
{
	public:

		ObjectSubscriber();

		virtual ~ObjectSubscriber();


	private:

		// callbacks

		gzVoid onNewObject(gzDistObject* object, gzDistSession* session);
		
		gzVoid onRemoveObject(gzDistObject* object, gzDistSession* session);

		gzVoid onUpdateAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session);

		gzVoid onSystemShutdown();


	private:

		// members

		gzDistSessionPtr m_session;

};


ObjectSubscriber::ObjectSubscriber() : gzDistClientInterface("subscriber")
{
	// initialize client
	initialize();

	// get the session (create it if it does not exist)
	m_session = getSession(HELLO_SESSION, TRUE);

	// join the session
	joinSession(m_session);

	// subscribe for objects
	subscribeObjects(gzDistObject::getClassType(), m_session);
}


ObjectSubscriber::~ObjectSubscriber()
{
	// Unsubscribe objects. Not really needed when the uninitialize() does it for us.
	unsubscribeObjects(gzDistObject::getClassType(), m_session);

	// Resign the session. Not really needed when the uninitialize() does it for us.
	resignSession(m_session);

	// Uninitialize client
	uninitialize();
}


gzVoid ObjectSubscriber::onNewObject(gzDistObject* object, gzDistSession* session)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "New object: %s", (const char*)object->getName());
	subscribeAttributeValue(HELLO_ATTRIBUTE, object);
}


gzVoid ObjectSubscriber::onRemoveObject(gzDistObject* object, gzDistSession* session)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Removed object: %s", (const char*)object->getName());
}


gzVoid ObjectSubscriber::onUpdateAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session)
{
	gzDistNotificationSetIterator iter(attributes);
	
	gzDistNotificationData* data = NULL;

	while (data = iter())
	{
		GZMESSAGE(GZ_MESSAGE_NOTICE, (const char*)data->getAttributeValue().getString());
	}
}


gzVoid ObjectSubscriber::onSystemShutdown()
{
	// in case of pre-mature shutdown
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

	// create subscriber client
	ObjectSubscriber* subscriber = new ObjectSubscriber;

	gzSleep(GZ_SLEEP_SECOND);

	// create publisher client
	ObjectPublisher* publisher = new ObjectPublisher;


	// run a while
	gzSleep(10 * GZ_SLEEP_SECOND);
	

	// delete the publisher
	delete publisher;

	gzSleep(GZ_SLEEP_SECOND);

	// delete the subscriber
	delete subscriber;

	// shut down and release manager
	manager->shutDown();
	manager = NULL;

	gzSleep(GZ_SLEEP_SECOND);

	return 0;

}

