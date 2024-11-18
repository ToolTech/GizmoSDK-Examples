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
// File			: gooble.cpp
// Module		: 
// Description	: Example of large numbers in GizmoBase
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
// Who	Date	Description						
//									
// AMO	040209	Created file 	
//
// ******************************************************************************

#include <iostream>
#include "gzGooble.h"

int main()
{
    std::cout << "Hello World!\n";

    gzGooble::setDecimalPrecision(60);

    gzGooble a("123.456789817264817248");

    gzGooble b("29835923857298357928357928357298523234.23423423742837628");


    std::cout << (const char*)a.round().asString()<<"+"<< (const char*)b.round().asString()<<"="<< (const char*)(a+b).round().asString()<<std::endl;

    gzGooble pi = gzGooble::pi();

    std::cout << "Pi=" << (const char*)pi.asString()<<std::endl;

}

