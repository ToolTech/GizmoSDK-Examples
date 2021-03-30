NAME=shaders
MAKEFILE=shaders.mak
FILES = shaders.obj  

INCLUDES = /I "..\..\include" 

LIBS ="..\..\lib\gzsystem.lib" \
	"..\..\lib\gzdb.lib" \
	"..\..\lib\gzgraph.lib" \
	"..\..\lib\gzimage.lib" \
	"..\..\lib\gzbase.lib"

!if EXIST (.\winbuildflags)
include winbuildflags
!endif

CPP=cl.exe
LINK=link.exe

"$(NAME).exe" : $(FILES)
	$(LINK) @<<
	$(LINKFLAGS) $(FILES) $(LIBS)
<<
	-@echo ---------- build of "$(NAME).exe" is OK ! ------------


.cpp.obj::
	$(CPP) @<<
	$(CPPOPTS) $(MEM_DEBUG) $(INCLUDES) $< 
<<


debug:
	-@echo CPPOPTS = /nologo /MT /W3 /Gm /GX /ZI /Od /D "GZ_DLL" /D "_DEBUG" /D "GZ_OPENGL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "GZ_DEBUG" /YX /FD /GZ /c > winbuildflags
	-@echo LINKFLAGS = libcmt.lib libcimt.lib opengl32.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"libcimtd.lib" /out:"$(NAME).exe" /pdbtype:sept >> winbuildflags
	-@echo #MEM_DEBUG =/D "GZ_MEM_DEBUG" >> winbuildflags
	-@echo ------------------ Made debug configuration -------------
	-@nmake /f $(MAKEFILE) clean
	-@echo ------------------ Made clean debug configuration -------
	
memtrace:
	-@echo CPPOPTS = /nologo /MT /W3 /Gm /GX /ZI /Od /D "GZ_DLL" /D "_DEBUG" /D "GZ_OPENGL" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "GZ_DEBUG" /YX /FD /GZ /c > winbuildflags
	-@echo LINKFLAGS = libcmt.lib libcimt.lib opengl32.lib netapi32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /nodefaultlib:"libcimtd.lib" /out:"$(NAME).exe" /pdbtype:sept >> winbuildflags
	-@echo MEM_DEBUG =/D "GZ_MEM_DEBUG" >> winbuildflags
	-@echo ------------------ Made mem debug configuration -------------
	-@nmake /f $(MAKEFILE) clean
	-@echo ------------------ Made clean mem debug configuration -------
	
release:
	-@echo CPPOPTS = /nologo /MT /W3 /GX /O2 /D "GZ_DLL" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "GZ_OPENGL" /YX /FD /c > winbuildflags
	-@echo LINKFLAGS = kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib netapi32.lib /nologo /subsystem:console /incremental:no /nodefaultlib:"libc.lib" /machine:I386 /out:"$(NAME).exe"  >> winbuildflags
	-@echo #MEM_DEBUG =/D "GZ_MEM_DEBUG" >> winbuildflags
	-@echo ------------------ Made release configuration -------------
	-@nmake /f $(MAKEFILE) clean
	-@echo ------------------ Made clean release configuration -------
	
clean:
       -@erase "$(FILES)"
       -@erase "vc60.*"
       -@erase "$(NAME).exe"
       -@erase "$(NAME).ilk"
       -@erase "$(NAME).pdb"
       -@erase "$(NAME).ncb"
       -@erase "$(NAME).opt"
       -@erase "$(NAME).plg"


