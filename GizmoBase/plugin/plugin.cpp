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
// File			: plugin.cpp
// Module		: 
// Description	: A sample plugin module
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
#include "stdio.h"
#define VERSIONSTR "MODULE: plugin ver 1.0"

#include "plugtype.h"

class MinModul : public gzDynamicMethodChain<MinModul,gzModule>
{
public:
	GZ_DECLARE_TYPE_INTERFACE;

	MinModul()
	{
		registerMethod(IID_DEBUG,IIDS_DEBUG,&MinModul::debug);
		registerDirectMethod(IID_DEBUG,s_debug);

		registerMethod(IID_VERSION,IIDS_VERSION,&MinModul::version);

		registerMethod(IID_PLUGTYPE,IIDS_PLUGTYPE,&MinModul::getMyPlugData);
	}

	gzVoid onEvent( gzModuleEvent event)
	{
		switch(event)
		{
			case GZ_MODULE_ADD:
				GZMESSAGE(GZ_MESSAGE_DEBUG,"Loading Plugin Module");
                m_data.m_member=LL(406806012537);
				break;

			case GZ_MODULE_REMOVE:
				GZMESSAGE(GZ_MESSAGE_DEBUG,"Unloading Plugin Module");
				break;
		}
	};
	
	static gzDynamicType GZ_CALLBACK s_debug(gzDynamicInvokeInterface *iface,GZ_DYNAMIC_ATTRIBUTE_LIST_IMP)
	{
		return ((MinModul *)iface)->debug(GZ_DYNAMIC_ATTRIBUTE_LIST_IMP_FORWARD);
	}
	
	gzDynamicType   debug(GZ_DYNAMIC_ATTRIBUTE_LIST)
	{
		gzString debugstr;
		gzTime  today;
		
		debugstr=gzString("Todays date is ")+gzString(today.day())+gzString("/")+gzString(today.month());
		
		sendEvent(IID_DEBUG,gzString("Test"));
				
		GZMESSAGE(GZ_MESSAGE_DEBUG|GZ_MESSAGE_ENCODING_NO_TRANSLATE,debugstr);
		
		return gzDynamicTypeVoid();
	}
	
	gzDynamicType   version(GZ_DYNAMIC_ATTRIBUTE_LIST)
	{
		GZMESSAGE(GZ_MESSAGE_DEBUG|GZ_MESSAGE_ENCODING_NO_TRANSLATE,VERSIONSTR);
		
		return gzDynamicTypeVoid();
	}

    gzDynamicType getMyPlugData(GZ_DYNAMIC_ATTRIBUTE_LIST)
    {
        return gzDynamicType(&m_data);
    }
        
    //----------- Member data --------------------

    MyPlugClass m_data;

};

GZ_DECLARE_TYPE_CHILD(gzModule,MinModul,"MyModule");

GZ_DECLARE_MODULE(MinModul);

