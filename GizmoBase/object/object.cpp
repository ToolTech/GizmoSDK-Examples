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
// File			: object.cpp
// Module		: 
// Description	: Example of gzObject in GizmoBase
// Author		: Anders Modén		
// Product		: GizmoBase 2.11.76
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
// AMO	040209	Created file 	
//
// ******************************************************************************

#include "gzBaseLibrary.h"

class MyObjectClass : public gzObject , public gzNameInterface
{
public:
	GZ_DECLARE_TYPE_INTERFACE;

	MyObjectClass(const gzString &name=GZ_EMPTY_STRING):gzNameInterface(name){};
	virtual ~MyObjectClass()
	{
		removeAllUserData();
	}


	// User data notifications
	virtual gzVoid onUserDataAttributeEvent( gzUserData *data , gzDynamicTypeEvent reason , const gzString &name) 
	{
		GZTRACE("Got userdata reason %d at 0x%x with name %s",reason,data,(const char *)name);
	}


	// Cloning

	virtual gzReference* clone() const
	{
		return new MyObjectClass(*this);
	}


	// Serializing

	virtual gzVoid write(gzSerializeAdapter *adapter)
	{
		gzSerializeString name=getName();

		name.write(adapter);

		gzObject::write(adapter);
	}

	virtual gzVoid read(gzSerializeAdapter *adapter)
	{
		gzSerializeString name;

		name.read(adapter);

		gzObject::read(adapter);

		setName(name);
	}

	
	virtual gzUInt32	getDataSize(gzSerializeAdapter *adapter=NULL) const 
	{ 
		gzSerializeString name=getName();

		return gzObject::getDataSize(adapter)+name.getDataSize(adapter);
	}
};

GZ_DECLARE_TYPE_CHILD(gzObject,MyObjectClass,"MyObjectClass");


int main(int argc , char *argv[] )
{
	try
	{
		gzMessage::setMessageLevel(GZ_MESSAGE_DEBUG);

		// Register a factory for the object

		gzUInt32 fid=gzObject::registerFactoryObject(new MyObjectClass);


		// Show how to create objects from a factory id

		gzRefPointer<MyObjectClass> pObject=gzDynamic_Cast<MyObjectClass>(gzObject::createFactoryObject(fid));

		if(pObject)
		{
			// init object
			pObject->setName("test");


			// Set up a attribute container as user data
			gzUserDataAttributeContainer *attribs=new gzUserDataAttributeContainer;

			pObject->addUserData("attribs",attribs);



			// Set some generic attributes in attrib container
			attribs->setAttribute("Age",36);
			attribs->setAttribute("Name",gzString("Anders"));


			gzSerializeAdapter *adapter=gzSerializeAdapter::getURLAdapter("test.dat",GZ_SERIALIZE_OUTPUT);

			if(adapter)
			{
				gzObject::writeObject(pObject,adapter);
				delete adapter;
			}
		}




		gzSerializeAdapter *adapter=gzSerializeAdapter::getURLAdapter("test.dat");

		if(adapter)
		{
			pObject=gzDynamic_Cast<MyObjectClass>(gzObject::readObject(adapter));
			delete adapter;
		}

		if(pObject)
		{
			gzUserDataAttributeContainer *attribs=gzDynamic_Cast<gzUserDataAttributeContainer>(pObject->getUserData("attribs"));

			gzDynamicType *dyntype;

			gzString name;

			if(attribs)
			{
				gzDynamicTypeContainerIterator iterator(*attribs);
	
				while(dyntype=iterator(&name))
				{
					GZTRACE("Got attribute %s of type %s",(const char *)name,(const char *)dyntype->getDynamicType());
				}
			}
		}


	}
	catch(gzBaseError &error)	// In case of exceptions thrown we want to print the message
	{
 		error.reportError();
	}

	return 0;
}

