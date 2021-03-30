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
// File			: coordinates (Program.cs)
// Module		: 
// Description	: Example of different allocations
// Author		: Anders Modén		
// Product		: Coordinate 2.10.9
//		
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
// AMO	210330	Created file        (2.10.9) 	
//
// ******************************************************************************

using System;

using GizmoSDK.GizmoBase;
using GizmoSDK.Coordinate;

namespace coordinates
{
    class Program
    {
        static void Main(string[] args)
        {
            GizmoSDK.GizmoBase.Platform.Initialize();
            GizmoSDK.Coordinate.Platform.Initialize();

            LatPos latpos = new(0.2, 1.1, 123);

            Coordinate converter = new();

            converter.SetLatPos(latpos);

            converter.GetCartPos(out CartPos cartpos);

            Console.WriteLine(cartpos.ToString()); 
        }
    }
}
