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
// File			: plugtest.cpp
// Module		: 
// Description	: Test usage of compiled plugin module
// Author		: Anders Modén          
// Product		: GizmoBase 2.12.199
//              
//
// NOTE:	GizmoBase is a platform abstraction utility layer for C++. It contains 
//			design patterns and C++ solutions for the advanced programmer.
//
//
// Revision History...                                                  
//                                                                      
// Who  Date    Description                                             
//                                                                      
// AMO  991002  Created file  
//
// ******************************************************************************

#include "gzModule.h"
#include <iostream>

#include "plugtype.h"

class MyPlugEventManager : public gzDynamicEventNotifyInterface
{
	gzVoid onEvent( gzDynamicEventInterface * source , gzUInt64 IID_event , GZ_DYNAMIC_ATTRIBUTE_LIST_IMP )
	{
		GZMESSAGE(GZ_MESSAGE_NOTICE,"Got Event with attribute '%s'",(const char *)a0.getString());
	}
};

int main(int argc , char *argv[] )
{
	gzModule *module;

	MyPlugEventManager manager;

	try
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

		module=gzModule::getModule("plugin");

		if(module)
		{
			module->addSubscriber(&manager);

			if(module->supportMethod(IID_VERSION))
				module->invokeMethod(IID_VERSION);

			if(module->supportMethod(IID_DEBUG))
				module->invokeMethod(IID_DEBUG);

			// Direct invoke

			gzDynamicMethod debug=module->getDirectMethod(IID_DEBUG);

			if(debug)
				debug(module,GZ_DYA_0);


			if(module->supportMethod(IID_PLUGTYPE))
			{
				MyPlugClass *myclass=(MyPlugClass *)(module->invokeMethod(IID_PLUGTYPE)).getPointer();
				GZMESSAGE(GZ_MESSAGE_NOTICE,"Got value %lld from module",(gzDouble)myclass->m_member);
			}

			module->unLoad();
		}
	}
	catch(gzBaseError &error)
	{
		error.reportError();
	}

	return 0;
}


