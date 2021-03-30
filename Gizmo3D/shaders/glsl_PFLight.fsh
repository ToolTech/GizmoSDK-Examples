/*****************************************************************************
 File		: glsl_PFLight.fsh
 Module		: 
 Description: Test fragment shader
 Author		: Anders Modén		
 Product	: Gizmo3D 1.2
		
 Copyright © 2003- Saab Training Systems AB, Sweden
			
 NOTE:	Gizmo3D is a high performance 3D Scene Graph and effect visualisation 
		C++ toolkit for Linux, Mac OS X, Windows (Win32) and IRIX® for  
		usage in Game or VisSim development.


 Revision History...							
									
 Who	Date	Description						
									
 AMO	040423	Created file 	

******************************************************************************/

uniform vec4 color;

uniform float renderTime;	// Built in by Gizmo3D = (seconds since hour started for current rendering)

varying vec3 test;

void main()
{
	// Just a simple value "color" collected from the gzGFXProgram local parameters

	gl_FragColor = vec4(test,1);

}