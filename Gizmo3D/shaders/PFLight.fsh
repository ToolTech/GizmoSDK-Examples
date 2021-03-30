!!ARBfp1.0
#*****************************************************************************
# File		: PFLight.fsh
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


TEMP lightVec, viewVec, reflVec, normal, attenuation, Len, finalCol, lightContrib, tex;
PARAM two = {2.0, 2.0, 2.0, 2.0};
PARAM m_one = {-1.0, -1.0, -1.0, -1.0};

TEX		tex, fragment.texcoord[0], texture[0], 2D;

#Normalize normal, lightvec and viewvec.
DP3		Len.w, fragment.texcoord[1], fragment.texcoord[1];
RSQ		lightVec.w, Len.w;
MUL		lightVec.xyz, fragment.texcoord[1], lightVec.w;

DP3		viewVec.w, fragment.texcoord[2], fragment.texcoord[2];
RSQ		viewVec.w, viewVec.w;
MUL		viewVec.xyz, fragment.texcoord[2], viewVec.w;

DP3		normal.w, fragment.texcoord[3], fragment.texcoord[3];
RSQ		normal.w, normal.w;
MUL		normal.xyz, fragment.texcoord[3], normal.w;

#Calculate attenuation.
MAD		attenuation, state.light[0].attenuation.z, Len.w, state.light[0].attenuation.x;
RCP		Len, lightVec.w;
MAD		attenuation, Len.w, state.light[0].attenuation.y, attenuation.x;
RCP		attenuation.x, attenuation.x;

#Diffuse
DP3_SAT	lightContrib.x, normal, lightVec;

#Specular
# Phong:
DP3		reflVec, lightVec, normal;
MUL		reflVec, reflVec, two;
MAD		reflVec, reflVec, normal, -lightVec;

DP3_SAT	lightContrib.y, reflVec, viewVec;

# Blinn:
#	ADD		reflVec, lightVec, viewVec;	# reflVec == Half-angle.
#	DP3		reflVec.w, reflVec, reflVec;
#	RSQ		reflVec.w, reflVec.w;
#	MUL		reflVec.xyz, reflVec, reflVec.w;
#	DP3		lightContrib.y, reflVec, normal;

MOV		lightContrib.w, state.material.shininess.x;

#Accelerates lighting computations
LIT_SAT	lightContrib, lightContrib;

MAD		finalCol, lightContrib.y, state.lightprod[0].diffuse, state.lightprod[0].ambient;

# Enable this line for textured models
MUL		finalCol, finalCol, tex;	# Texture?

MAD		finalCol, lightContrib.z, state.lightprod[0].specular, finalCol;
MAD		finalCol, finalCol, attenuation.x, state.material.emission;
ADD		result.color.xyz, finalCol, state.lightmodel.scenecolor;
MOV		result.color.w, state.material.diffuse.w;
END