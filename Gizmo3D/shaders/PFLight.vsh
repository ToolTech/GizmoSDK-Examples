!!ARBvp1.0
#*****************************************************************************
# File		: PFLight.vsh
# Module		: 
# Description	: Fixed function 1 point light replacement vertex program.
# Author		: David Luotonen, Anders Modén		
# Product		: Gizmo3D 1.1 Beta 51 (Shader)
#		
# Copyright © 2003- Saab Training Systems AB, Sweden
#			
# NOTE:	Gizmo3D is a high performance 3D Scene Graph and effect visualisation 
#		C++ toolkit for Linux, Mac OS X, Windows (Win32) and IRIX® for  
#		usage in Game or VisSim development.
#
#
# Revision History...							
#									
# Who	Date	Description						
#									
# DLU	030715	Created file 	
#
#******************************************************************************

OPTION ARB_position_invariant;

PARAM lightPos  = program.local[0];
PARAM cameraPos = program.local[1];
PARAM m[4]={state.matrix.modelview};
PARAM mvinv[4]={state.matrix.modelview.invtrans};

TEMP EyeCamPos, EyeNormal, EyeVertex;

#Vertex position in eye space...
DP4	EyeVertex.x, m[0], vertex.position;
DP4	EyeVertex.y, m[1], vertex.position;
DP4	EyeVertex.z, m[2], vertex.position;

#Transform the camera pos to eye space.
DP4	EyeCamPos.x, m[0], cameraPos;
DP4	EyeCamPos.y, m[1], cameraPos;
DP4	EyeCamPos.z, m[2], cameraPos;

#Transform the normal to eye space.
DP3	EyeNormal.x, mvinv[0], vertex.normal;
DP3	EyeNormal.y, mvinv[1], vertex.normal;
DP3	EyeNormal.z, mvinv[2], vertex.normal;

#Normalize
DP3 EyeNormal.w, EyeNormal, EyeNormal;
RSQ EyeNormal.w, EyeNormal.w;
MUL EyeNormal, EyeNormal, EyeNormal.w;

MOV	result.texcoord[0], vertex.texcoord[0];
SUB	result.texcoord[1], state.light[0].position, EyeVertex;
SUB	result.texcoord[2], EyeCamPos, EyeVertex;
MOV result.texcoord[3], EyeNormal;
END