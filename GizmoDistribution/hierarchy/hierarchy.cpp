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
// File			: hierarchy.cpp
// Module		: hierarchy
// Description	: hierarchy (example application)
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
// XAA	040420	Created file 	
//
//*****************************************************************************

// Includes
#include "gzBaseLibrary.h"
#include "gzDistLibrary.h"


// Common constants
static const gzString EXAMPLE_SESSION = "example";


//--------------------------- Object hierarchy --------------------------------
//
//                            ----------------
//                            | gzDistObject |
//                            ----------------
//                                    ^
//                                    |
//                -----------------------------------------
//                |                                       |
//           -----------                             -----------
//           | ObjectA |                             | ObjectB |
//           -----------                             -----------
//                ^                                       ^
//                |                                       |
//       -------------------                     -------------------
//       |                 |                     |                 |
// ------------       ------------         ------------       ------------
// | ObjectA1 |       | ObjectA2 |         | ObjectB1 |       | ObjectB2 |
// ------------       ------------         ------------       ------------
//
//

class ObjectA : public gzDistObject
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		ObjectA(const gzString& name) : gzDistObject(name) { };

		virtual ~ObjectA() { };

		virtual gzReference* clone() const
		{
			return new ObjectA(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(gzDistObject, ObjectA, "ObjectA");


class ObjectA1 : public ObjectA
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		ObjectA1(const gzString& name) : ObjectA(name) { };

		virtual ~ObjectA1() { };

		virtual gzReference* clone() const
		{
			return new ObjectA1(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(ObjectA, ObjectA1, "ObjectA1");


class ObjectA2 : public ObjectA
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		ObjectA2(const gzString& name) : ObjectA(name) { };

		virtual ~ObjectA2() { };

		virtual gzReference* clone() const
		{
			return new ObjectA2(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(ObjectA, ObjectA2, "ObjectA2");


class ObjectB : public gzDistObject
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		ObjectB(const gzString& name) : gzDistObject(name) { };

		virtual ~ObjectB() { };

		virtual gzReference* clone() const
		{
			return new ObjectB(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(gzDistObject, ObjectB, "ObjectB");


class ObjectB1 : public ObjectB
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		ObjectB1(const gzString& name) : ObjectB(name) { };

		virtual ~ObjectB1() { };

		virtual gzReference* clone() const
		{
			return new ObjectB1(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(ObjectB, ObjectB1, "ObjectB1");


class ObjectB2 : public ObjectB
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		ObjectB2(const gzString& name) : ObjectB(name) { };

		virtual ~ObjectB2() { };

		virtual gzReference* clone() const
		{
			return new ObjectB2(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(ObjectB, ObjectB2, "ObjectB2");


//--------------------------- Event hierarchy ---------------------------------
//
//                            ---------------
//                            | gzDistEvent |
//                            ---------------
//                                   ^
//                                   |
//                ---------------------------------------
//                |                                     |
//           ----------                             ----------
//           | EventA |                             | EventB |
//           ----------                             ----------
//                ^                                     ^
//                |                                     |
//       -------------------                   -------------------
//       |                 |                   |                 |
//  -----------       -----------         -----------       -----------
//  | EventA1 |       | EventA2 |         | EventB1 |       | EventB2 |
//  -----------       -----------         -----------       -----------
//
//

class EventA : public gzDistEvent
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		EventA() { };

		virtual ~EventA() { };

		virtual gzReference* clone() const
		{
			return new EventA(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(gzDistEvent, EventA, "EventA");


class EventA1 : public EventA
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		EventA1() { };

		virtual ~EventA1() { };

		virtual gzReference* clone() const
		{
			return new EventA1(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(EventA, EventA1, "EventA1");


class EventA2 : public EventA
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		EventA2() { };

		virtual ~EventA2() { };

		virtual gzReference* clone() const
		{
			return new EventA2(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(EventA, EventA2, "EventA2");


class EventB : public gzDistEvent
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		EventB() { };

		virtual ~EventB() { };

		virtual gzReference* clone() const
		{
			return new EventB(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(gzDistEvent, EventB, "EventB");


class EventB1 : public EventB
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		EventB1() { };

		virtual ~EventB1() { };

		virtual gzReference* clone() const
		{
			return new EventB1(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(EventB, EventB1, "EventB1");


class EventB2 : public EventB
{
	public:

		GZ_DECLARE_TYPE_INTERFACE;

		EventB2() { };

		virtual ~EventB2() { };

		virtual gzReference* clone() const
		{
			return new EventB2(*this);
		}

};

GZ_DECLARE_TYPE_CHILD(EventB, EventB2, "EventB2");


//------------------------------- Publisher -----------------------------------

// A client that publish objects and events of different types
class Publisher : public gzDistClientInterface
{
	public:

		Publisher();

		virtual ~Publisher();


	private:

		// callbacks

		gzVoid onClientTick();

		gzVoid onSystemShutdown();


	private:

		// members

		gzDistSessionPtr m_session;

		gzUInt32 m_tickCount;

};


Publisher::Publisher()
 :	gzDistClientInterface("publisher"),
	m_tickCount(0)
{
	// initialize client
	initialize(1);

	// create and join session
	m_session = getSession(EXAMPLE_SESSION, TRUE);
	joinSession(m_session);


	//  add an object of every type
	addObject(new gzDistObject("base"), m_session);
	addObject(new ObjectA("a"), m_session);
	addObject(new ObjectA1("a1"), m_session);
	addObject(new ObjectA2("a2"), m_session);
	addObject(new ObjectB("b"), m_session);
	addObject(new ObjectB1("b1"), m_session);
	addObject(new ObjectB2("b2"), m_session);

}


Publisher::~Publisher()
{
	// resign the session
	resignSession(m_session);

	// uninitialize client
	uninitialize();
}


gzVoid Publisher::onClientTick()
{
	// send an event of every type
	sendEvent(new gzDistEvent, m_session);
	sendEvent(new EventA, m_session);
	sendEvent(new EventA1, m_session);
	sendEvent(new EventA2, m_session);
	sendEvent(new EventB, m_session);
	sendEvent(new EventB1, m_session);
	sendEvent(new EventB2, m_session);

}


gzVoid Publisher::onSystemShutdown()
{
	// in case of pre-mature shutdown
	m_session = NULL;	
}


//------------------------------- Subscriber ----------------------------------

// A client that subscribe obejects and attributes
class Subscriber : public gzDistClientInterface
{
	public:

		Subscriber();

		virtual ~Subscriber();


	private:

		// callbacks

		gzVoid onClientTick();

		gzVoid onEvent(gzDistEvent* event);
		
		gzVoid onNewObject(gzDistObject* object, gzDistSession* session);

		gzVoid onRemoveObject(gzDistObject* object, gzDistSession* session);

		gzVoid onSystemShutdown();


	private:

		// members

		gzDistSessionPtr m_session;

		gzUInt32 m_tickCount;

};


Subscriber::Subscriber()
 :	gzDistClientInterface("subscriber"),
	m_tickCount(0)
{
	// initialize client
	initialize(1);

	// create and join session 
	m_session = getSession(EXAMPLE_SESSION, TRUE);
	joinSession(m_session);

}


Subscriber::~Subscriber()
{
	// resign the session
	resignSession(m_session);

	// uninitialize client
	uninitialize();
}


gzVoid Subscriber::onClientTick()
{
	if (! m_session)
	{
		return;
	}

	switch (m_tickCount)
	{
		case 0:
		{
			// EventA and events below will be received
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Subcribing EventA");
			subscribeEvents(EventA::getClassType(), m_session);

		}
		break;

		case 2:
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Unsubcribing EventA");
			unsubscribeEvents(EventA::getClassType(), m_session);

			// EventB1 will be received
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Subcribing EventB1");
			subscribeEvents(EventB1::getClassType(), m_session);
		}
		break;		

		case 4:
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Unsubcribing EventB1");
			unsubscribeEvents(EventB1::getClassType(), m_session);

			// ObjectA and objects below will be received as new
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Subcribing ObjectA");
			subscribeObjects(ObjectA::getClassType(), m_session, TRUE);

		}
		break;

		case 6:
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Unsubcribing ObjectA");
			unsubscribeObjects(ObjectA::getClassType(), m_session);

			// ObjectA1 will be received as new
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Subcribing ObjectA2");
			subscribeObjects(ObjectA2::getClassType(), m_session, TRUE);

		}
		break;

		case 8:
		{
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Unsubcribing ObjectA2");
			unsubscribeObjects(ObjectA2::getClassType(), m_session);

			// All objects will be received as new
			GZMESSAGE(GZ_MESSAGE_NOTICE, "Subcribing gzDistObject");
			subscribeObjects(gzDistObject::getClassType(), m_session, TRUE);

		}
		break;

	}

	++m_tickCount;

}


gzVoid Subscriber::onEvent(gzDistEvent* event)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Received event of type '%s'", event->getTypeName());
}

		
gzVoid Subscriber::onNewObject(gzDistObject* object, gzDistSession* session)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "New object of type '%s'", object->getTypeName());
}


gzVoid Subscriber::onRemoveObject(gzDistObject* object, gzDistSession* session)
{
	GZMESSAGE(GZ_MESSAGE_NOTICE, "Removed object of type '%s'", object->getTypeName());
}


gzVoid Subscriber::onSystemShutdown()
{
	// in case of pre-mature shutdown
	m_session = NULL;	
}


//---------------------------------- main -------------------------------------

// Application entry point
int main(int argc, char* argv[])
{
	// setup default message receiver
	gzMessage::setMessageLevel(GZ_MESSAGE_NOTICE);

	// create manager and start
	gzDistManagerPtr manager = gzDistManager::getManager(TRUE);
	manager->start();

	// register factories for all custom object types
	// factories are used by the kernel to create
	// new objects on global sessions
	manager->registerFactory(new ObjectA(GZ_EMPTY_STRING));
	manager->registerFactory(new ObjectA1(GZ_EMPTY_STRING));
	manager->registerFactory(new ObjectA2(GZ_EMPTY_STRING));
	manager->registerFactory(new ObjectB(GZ_EMPTY_STRING));
	manager->registerFactory(new ObjectB1(GZ_EMPTY_STRING));
	manager->registerFactory(new ObjectB2(GZ_EMPTY_STRING));

	// register factories for all custom event types
	// factories are used by the kernel to create
	// events on global sessions
	manager->registerFactory(new EventA);
	manager->registerFactory(new EventA1);
	manager->registerFactory(new EventA2);
	manager->registerFactory(new EventB);
	manager->registerFactory(new EventB1);
	manager->registerFactory(new EventB2);

	// create clients
	Subscriber* subscriber = new Subscriber;
	gzSleep(GZ_SLEEP_SECOND);
	Publisher* publisher = new Publisher;


	// run a while
	gzSleep(10 * GZ_SLEEP_SECOND);
	

	// delete clients
	delete publisher;
	gzSleep(GZ_SLEEP_SECOND);
	delete subscriber;

	// shut down and release manager
	manager->shutDown();
	manager = NULL;

	gzSleep(GZ_SLEEP_SECOND);

	return 0;

}

