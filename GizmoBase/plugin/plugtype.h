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
// File         : plugin.h
// Module		: 
// Description  : A sample user defined type
// Author		: Anders Modén          
// Product		: Gizmo3D 2.10.9
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
// Who  Date    Description                                             
//                                                                      
// AMO  000822  Created file  
//
// ******************************************************************************

#include "gzBase.h"

const gzInt64 IID_PLUGTYPE = 99;
const gzString   IIDS_PLUGTYPE= "plugtype";

// a sample class that should be shared by the two modules

class MyPlugClass
{
public:

    gzInt64 m_member;

};


