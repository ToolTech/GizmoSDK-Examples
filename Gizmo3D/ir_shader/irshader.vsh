!!ARBvp1.0
#*****************************************************************************
# File			: irshader.vsh
# Module		: 
# Description	: Fixed function 1 point light replacement vertex program.
# Author		: Lisa Johansson		
# Product		: Gizmo3D 1.2
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
# LJH	040510	Created file 	
#
#******************************************************************************

PARAM mvp[4]={state.matrix.mvp};


OUTPUT	oPos=result.position;		

OUTPUT	oColor=result.color;

ATTRIB  iPos=vertex.position;		
ATTRIB  iColor=vertex.color;		
ATTRIB	irColor=vertex.attrib[4];	


DP4	oPos.x, mvp[0], iPos;
DP4	oPos.y, mvp[1], iPos;
DP4	oPos.z, mvp[2], iPos;
DP4	oPos.w, mvp[3], iPos;

MOV oColor,irColor;

END
