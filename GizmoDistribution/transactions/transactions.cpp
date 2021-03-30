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
// File			: transactions.cpp
// Module		: transactions
// Description	: transactions (example application)
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
static const gzString EXAMPLE_SESSION = "example";
static const gzString EXAMPLE_OBJECT = "example";


//-----------------------------------------------------------------------------

// A client that publish objects
class Publisher : public gzDistClientInterface
{
	public:

		Publisher();

		virtual ~Publisher();


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

		gzUInt32 m_tickCount;

};


Publisher::Publisher()
 :	gzDistClientInterface("publisher"),
	m_tickCount(0)
{
	// initialize client
	initialize(2);

	// create and join session 
	m_session = getSession(EXAMPLE_SESSION, TRUE);
	joinSession(m_session);

	// subscribe for objects
	subscribeObjects(gzDistObject::getClassType(), m_session);

	// add  object
	addObject(new gzDistObject(EXAMPLE_OBJECT), m_session);
	
}


Publisher::~Publisher()
{
	// unsubscribe objects
	unsubscribeObjects(gzDistObject::getClassType(), m_session);

	// resign the session
	resignSession(m_session);

	// uninitialize client
	uninitialize();
}


gzVoid Publisher::onNewObject(gzDistObject* object, gzDistSession* session)
{
	if (!m_object)
	{
		m_object = object;
	}
}


gzVoid Publisher::onRemoveObject(gzDistObject* object, gzDistSession* session)
{
	if (object == m_object)
	{
		m_object = NULL;
	}
}


gzVoid Publisher::onClientTick()
{
	if (!m_object)
	{
		return;
	}

	if (m_tickCount%5 != 0)
	{
		// add and/or update attributes

		// notifications will be received on callbacks
		// onNewAttributes() for new attributes and
		// onUpdateAttributes for updated attributes
		
		gzDistTransactionPtr transaction = new gzDistTransaction;
		
		gzUInt32 count = (gzUInt32)(10.0 * gzRandom() + 1.0);

		for (gzUInt32 i = 0; i < count; i++)
		{
			transaction->setAttributeValue(gzString(i), gzString(m_tickCount));
		}

		GZMESSAGE(GZ_MESSAGE_NOTICE, "%u attributes in update transaction", count);

		updateObject(transaction, m_object);

	}
	else
	{
		// remove attributes 

		// notification will be received on callback onRemoveAttributes()

		gzDistTransactionPtr transaction = new gzDistTransaction;
		
		gzUInt32 count = (gzUInt32)(10.0 * gzRandom() + 1.0);

		for (gzUInt32 i = 0; i < count; i++)
		{
			transaction->setAttributeName(gzString(i));
		}

		GZMESSAGE(GZ_MESSAGE_NOTICE, "%u attributes in remove transaction", count);

		removeAttributes(transaction, m_object);

	}

	++m_tickCount;
}


gzVoid Publisher::onSystemShutdown()
{
	// GizmoDistribution is shutting down
	// free all references to enable the library to be unloaded properly
	// it is recommended to always implement this callback
	m_object = NULL;
	m_session = NULL;	
}


//-----------------------------------------------------------------------------

// A client that subscribe for objects and attributes
class Subscriber : public gzDistClientInterface
{
	public:

		Subscriber();

		virtual ~Subscriber();


	private:

		// callbacks

		gzVoid onNewObject(gzDistObject* object, gzDistSession* session);
		
		gzVoid onRemoveObject(gzDistObject* object, gzDistSession* session);

		gzVoid onNewAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session);
		
		gzVoid onUpdateAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session);

		gzVoid onRemoveAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session);

		gzVoid onSystemShutdown();


	private:

		// members

		gzDistSessionPtr m_session;

};


Subscriber::Subscriber()
 : gzDistClientInterface("subscriber")
{
	// initialize client
	initialize();

	// create and join session
	m_session = getSession(EXAMPLE_SESSION, TRUE);
	joinSession(m_session);

	// subscribe for objects
	subscribeObjects(gzDistObject::getClassType(), m_session);
}


Subscriber::~Subscriber()
{
	// unsubscribe objects
	unsubscribeObjects(gzDistObject::getClassType(), m_session);

	// resign the session
	resignSession(m_session);

	// uninitialize client
	uninitialize();
}


gzVoid Subscriber::onNewObject(gzDistObject* object, gzDistSession* session)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "New object '%s' on session '%s'",
				(const char*)object->getName(), (const char*)object->getSession()->getName());
	
	// subscribe for attributes on the new object
	// notifications will be received in the onNewAttributes() and onRemoveAttributes()
	// callbacks when attributes are created or removed from this object
	subscribeAttributes(object, TRUE);
	
}


gzVoid Subscriber::onRemoveObject(gzDistObject* object, gzDistSession* session)
{
	// an object was removed
	// all subscriptions associated with the object are automatically removed

	GZMESSAGE(GZ_MESSAGE_NOTICE, "Removed object '%s' from session '%s'",
				(const char*)object->getName(), (const char*)session->getName());
}


gzVoid Subscriber::onNewAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session)
{
	// attributes were added to an object

	GZMESSAGE(GZ_MESSAGE_NOTICE, "%u new attributes on object '%s' on session '%s'",
				attributes.entries(), (const char*)object->getName(), (const char*)object->getSession()->getName());


	gzDistNotificationSetIterator iter(attributes);

	gzDistNotificationData* data;

	while (data = iter())
	{
		// subscribe for updates of the attribute value
		// notifications will be received in the onUpdateAttributes() callback
		subscribeAttributeValue(data->getAttributeName(), object);
	}
	
}


gzVoid Subscriber::onRemoveAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session)
{
	// attributes were removed form an object

	GZMESSAGE(GZ_MESSAGE_NOTICE, "%u removed attributes from object '%s' on session '%s'",
				attributes.entries(), (const char*)object->getName(), (const char*)object->getSession()->getName());
}


gzVoid Subscriber::onUpdateAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session)
{
	// attribute values were updated

	GZMESSAGE(GZ_MESSAGE_NOTICE, "%u attributes updated on object '%s' on session '%s'",
				attributes.entries(), (const char*)object->getName(), (const char*)object->getSession()->getName());
}


gzVoid Subscriber::onSystemShutdown()
{
	// GizmoDistribution is shutting down
	// free all references to enable the library to be unloaded properly
	// it is recommended to always implement this callback
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

	// create publisher and subscriber
	Subscriber* subscriberA =  new Subscriber;
	gzSleep(GZ_SLEEP_SECOND);
	Publisher* publisherA =  new Publisher;


	// wait for return
	getchar();


	// delete publisher and subscriber
	delete publisherA;
	gzSleep(GZ_SLEEP_SECOND);
	delete subscriberA;


	// shut down and release manager
	manager->shutDown();
	manager = NULL;

	gzSleep(GZ_SLEEP_SECOND);

	return 0;

}

