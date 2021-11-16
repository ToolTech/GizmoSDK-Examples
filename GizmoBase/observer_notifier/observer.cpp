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
// File			: observer.cpp
// Module		: 
// Description	: Test implementation of how to use the observer notifier pattern
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
// Who	Date	Description						
//									
// AMO	031022	Created file 	
//
// ******************************************************************************

#include "gzBaseLibrary.h"	// All includes


// A class to hold data during a notification
// The data container can like in this case send a reference of data, but
// you can also use notifications with containment by value

class MyObserverData : public gzReference		
{
public:

	MyObserverData(gzInt32 data):m_data(data){};

	virtual ~MyObserverData(){};

	gzInt32 m_data;	// Just a public member of data

};

// We need to declare a forward class info for the template
class MyObserver;	


// A class that notifies others
// The class sends a direct or posted message to all its subscribers
class MyNotifier : public gzNotifyMaster<MyNotifier,MyObserver,gzRefPointer<MyObserverData> >
{
public:

	// With this optional callback you can get info about subscribers of your instance
	virtual gzVoid onNotifyStatus(MyObserver *slave,gzNotifyState state)
	{
		switch(state)
		{

			case GZ_NOTIFY_ADD_SUBSCRIBER :

				GZMESSAGE(GZ_MESSAGE_DEBUG,"Notifier:Added an observer to me");
				break;

			case GZ_NOTIFY_REMOVE_SUBSCRIBER :

				GZMESSAGE(GZ_MESSAGE_DEBUG,"Notifier:Removed an observer from me");
				break;

			case GZ_NOTIFY_SLAVE_TERMINATE :

				GZMESSAGE(GZ_MESSAGE_DEBUG,"Notifier:Terminated an observer of me");
				break;

		}
		
	}
};

// A class that gets notification from a notifier.
// The observer subscribes a myNotifier class and get notifications from that instance.
class MyObserver : public gzNotifySlave<MyNotifier,MyObserver,gzRefPointer<MyObserverData> >
{
public:

	virtual gzVoid onNotify(MyNotifier *master,gzRefPointer<MyObserverData> &data,gzUInt32 senderClass)
	{
		if(data.get())
			GZMESSAGE(GZ_MESSAGE_DEBUG,"Observer (%x):Got data %d class %d",this,data->m_data,senderClass);

	}

	virtual gzVoid onNotifyStatus(MyNotifier *master,MyObserver *slave,gzNotifyState state)
	{
		switch(state)
		{

			case GZ_NOTIFY_ADD_SUBSCRIBER :

				GZMESSAGE(GZ_MESSAGE_DEBUG,"Observer (%x):Added an observer (%x) to my master",this,slave);
				break;

			case GZ_NOTIFY_REMOVE_SUBSCRIBER :

				GZMESSAGE(GZ_MESSAGE_DEBUG,"Observer (%x):Removed an observer (%x) from my master",this,slave);
				break;

			case GZ_NOTIFY_MASTER_TERMINATE :

				GZMESSAGE(GZ_MESSAGE_DEBUG,"Observer (%x):Terminated a master of me",this);
				break;

		}
	}
};


int main(int argc , char *argv[] )
{
	try		// To catch all Gizmo3D exceptions
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

		// Create an observer and a notifier instance

		MyObserver *observer=new MyObserver;

		MyObserver *def_observer=new MyObserver;

		MyNotifier *notifier=new MyNotifier;

		// Add observer to notifier as a subscriber of certain data

		notifier->addSubscriber(observer,97);	// Add for class data type 97
		notifier->addSubscriber(observer,98);	// Add for class data type 98

		// Add Observer as a default observer

		notifier->addSubscriber(def_observer);	// Gets all data

		gzRefPointer<MyObserverData> data=new MyObserverData(43);

		notifier->notifyData(data);		// Notify all default subscribers
		notifier->notifyData(data,97);	// Notify class data 97
		notifier->notifyData(data,99);	// Notify class data 99

		// test to exchange these lines to see the effect of deletion

		delete notifier;

		delete observer;

	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
		error.reportError();
	}

	return 0;
}


