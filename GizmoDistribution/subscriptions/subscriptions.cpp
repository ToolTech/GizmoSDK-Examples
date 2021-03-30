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
// File			: subscriptions.cpp
// Module		: subscriptions
// Description	: subscriptions (example application)
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
// XAA	040419	Created file 	
//
//*****************************************************************************

// Includes
#include <stdio.h>
#include "gzBaseLibrary.h"
#include "gzDistLibrary.h"

// Session name
static const gzString A_SESSION = "A";
static const gzString B_SESSION = "B";
static const gzString C_SESSION = "C";


//----------------------------- Publisher -------------------------------

// A client that publish events, objects and attribute updates
class Publisher : public gzDistClientInterface
{
	public:

		explicit Publisher(const gzString& sessionName);

		virtual ~Publisher();


	private:

		// thread ticker callback
				
		gzVoid onClientTick();


		// notification callbacks

		gzVoid onNewObject(gzDistObject* object, gzDistSession* session);

		gzVoid onRemoveObject(gzDistObject* object, gzDistSession* session);

		gzVoid onSystemShutdown();


	private:

		// members

		gzUInt32 m_tickCount;

		gzBool m_add;

		gzString m_sessionName;

		gzDistSessionPtr m_session;

		gzRefList<gzDistObject> m_objects;

};


Publisher::Publisher(const gzString& sessionName)
 :	gzDistClientInterface(sessionName + gzString("_publisher")),
	m_tickCount(0),
	m_add(FALSE),
	m_sessionName(sessionName)
{
	// initialize client
	initialize(2);

	// create session
	m_session = getSession(sessionName, TRUE);
	joinSession(m_session);
	subscribeObjects(gzDistObject::getClassType(), m_session);

}


Publisher::~Publisher()
{
	// uninitialize client
	uninitialize();
}


gzVoid Publisher::onClientTick()
{
	if (!m_session)
	{
		return;
	}

	if (m_tickCount%5 == 0)
	{
		// add / remove objects

		if (m_add)
		{
			// first add 4 objects...

			if (m_objects.entries() < 5)
			{
				gzDistObjectPtr object = new gzDistObject(m_sessionName + gzString(m_objects.entries()));
				object->setAttributeValue(gzString(m_tickCount%5), gzString(m_tickCount));
				addObject(object, m_session);
			}
			else
			{
				m_add = FALSE;
			}
		}
		else
		{
			// and then remove them

			if (m_objects.entries())
			{
				gzDistObjectPtr object = m_objects.first();
				removeObject(object);
			}
			else
			{
				m_add = TRUE;
			}
		}
	}
	else
	{
		// add / update attributes of the current objects

		gzListIterator<gzDistObject> iter(m_objects);
		
		gzDistObject* object;
		
		while (object = iter())
		{
			updateObject(gzString(m_tickCount%5), gzString(m_tickCount), object);
		}
	}


	// send an event
	gzDistEventPtr event = new gzDistEvent;
	sendEvent(event, m_session);


	// increment tick counter
	++m_tickCount;

}


gzVoid Publisher::onNewObject(gzDistObject* object, gzDistSession* session)
{
	m_objects.insert(object);
}


gzVoid Publisher::onRemoveObject(gzDistObject* object, gzDistSession* session)
{
	m_objects.remove(object);
}


gzVoid Publisher::onSystemShutdown()
{
	// GizmoDistribution is shutting down
	// free all references to enable the library to be unloaded properly
	// it is recommended to always implement this callback
	m_session = NULL;
	m_objects.clear();
}


//------------------------------- Subscriber ----------------------------------

// A client that subscribe for events, objects and attributes
class Subscriber : public gzDistClientInterface
{
	public:

		Subscriber(const gzString& sessionName);

		virtual ~Subscriber();


	private:

		// notification callbacks

		gzVoid onNewSession(gzDistSession* session);

		gzVoid onRemoveSession(gzDistSession* session);

		gzVoid onEvent(gzDistEvent* event);
		
		gzVoid onNewObject(gzDistObject* object, gzDistSession* session);
		
		gzVoid onRemoveObject(gzDistObject* object, gzDistSession* session);

		gzVoid onNewAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session);

		gzVoid onRemoveAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session);

		gzVoid onUpdateAttributes(const gzDistNotificationSet& attributes, gzDistObject* object, gzDistSession* session);

		gzVoid onSystemShutdown();


	private:

		// members

		gzString m_sessionName;

		gzDistSessionPtr m_session;

};


Subscriber::Subscriber(const gzString& sessionName)
 :	gzDistClientInterface(sessionName + gzString("_subscriber")),
	m_sessionName(sessionName)
{
	// initialize client
	initialize();

	// subscribe for sessions
	// notifications will be received in onNewSession() and onRemoveSession()
	// callbacks when sessions are created or removed from this process
	subscribeSessions();

}


Subscriber::~Subscriber()
{
	// uninitialize client
	uninitialize();
}


gzVoid Subscriber::onNewSession(gzDistSession* session)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "New session '%s'", (const char*)session->getName());

	
	if (session->getName() == m_sessionName)
	{
		// join the new session
		joinSession(session);

		// subscribe for objects on the session
		// notifications will be received in onNewObject() and onRemoveObject()
		// callbacks when objects are created or removed from this session
		subscribeObjects(gzDistObject::getClassType(), session);

		// subscribe for events on the session
		// notifications will be received in the onEvent() callback
		// when events are sent on this session
		subscribeEvents(gzDistEvent::getClassType(), session);

		m_session = session;

	}
}


gzVoid Subscriber::onRemoveSession(gzDistSession* session)
{
	// a session was removed from this process
	// this will happen when a session no longer has
	// any local clients joined to it

	GZMESSAGE(GZ_MESSAGE_NOTICE, "Removed session '%s'", (const char*)session->getName());
}


gzVoid Subscriber::onEvent(gzDistEvent* event)
{
	// an event was received

	GZMESSAGE(GZ_MESSAGE_NOTICE, "Event received on sessions '%s'",
				(const char*)event->getSession()->getName());
}


gzVoid Subscriber::onNewObject(gzDistObject* object, gzDistSession* session)
{
	// an object has been added

	GZMESSAGE(GZ_MESSAGE_NOTICE, "New object '%s' on session '%s'",
				(const char*)object->getName(), (const char*)object->getSession()->getName());
	
	// subscribe for attributes on the new object
	// notifications will be received in the onNewAttributes() and onRemoveAttributes()
	// callbacks when attributes are created or removed from this object
	subscribeAttributes(object);
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


//--------------------------------- main --------------------------------------

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
	Subscriber* subscriberA =  new Subscriber(A_SESSION);
	gzSleep(GZ_SLEEP_SECOND);
	Publisher* publisherA =  new Publisher(A_SESSION);


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

