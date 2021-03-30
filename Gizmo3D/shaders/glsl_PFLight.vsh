/*****************************************************************************
 File		: glsl_PFLight.vsh
 Module		: 
 Description: Test vertex shader
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

uniform mat4 currentInvertedTransform;		// Built in by Gizmo3D = inv(gl_ModelViewMatrix)
uniform mat4 mainInvertedProjTransform;		// Built in by Gizmo3D = inv(gl_ProjectionMatrix)

varying vec3 test;

attribute vec3 tangent;

void main()
{
	
	// Just the standard transform but with a mix of all possible transforms

	//gl_Position=gl_ModelViewProjectionMatrix*mainInvertedProjTransform*gl_ProjectionMatrix*currentInvertedTransform*gl_ModelViewMatrix*gl_Vertex;

	test=tangent;

	gl_Position=ftransform();
	
}