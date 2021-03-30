

INCLUDEPATH_BASE = -I../../../GizmoBase/include
INCLUDEPATH_3D = -I../../include

LIBPATH_BASE = ../../../GizmoBase/lib
LIBPATH_3D = ../../lib

DEPLIBS =  $(LIBPATH_BASE)/libgzbase$(DP_EXT)$(SO_EXT) $(LIBPATH_3D)/libgzgraph$(DP_EXT)$(SO_EXT) $(LIBPATH_3D)/libgzimage$(DP_EXT)$(SO_EXT) $(LIBPATH_3D)/libgzdb$(DP_EXT)$(SO_EXT) $(LIBPATH_3D)/libgzsystem$(DP_EXT)$(SO_EXT)

LIBS = -lgzsystem$(DP_EXT) -lgzdb$(DP_EXT) -lgzgraph$(DP_EXT) -lgzimage$(DP_EXT) -lgzbase$(DP_EXT)


ENAME=$(EDIR)/$(NAME)

%.o : %.cpp
	test -f compilerflags ||  make help
	test -f buildflags ||  make help
	$(CXX) $(DP_FLAG) $(CXXOPTS) $(DBG_EXT) $(OPTI_EXT) $(DEBUG) $(EXTRA_CXXOPTS) $(MEM_DEBUG) $(INCLUDEPATH_BASE) $(INCLUDEPATH_3D) $(EXTRA_INCLUDE) -c $< -o $@

$(ENAME): $(FILES)  $(DEPLIBS)
	test -d $(EDIR) || mkdir -p $(EDIR)
	$(CXX) $(DP_FLAG) $(DBG_EXT) $(LINKOPTS) -o $(ENAME) $(FILES) $(EXTRA_LINKOPTS) -L$(LIBPATH_BASE) -L$(LIBPATH_3D) $(LIBS) $(EXT_LIBS) $(EXTRA_LIBS)
	$(DEPUPDATE)
	# ------------------ done making example ---------------- 

sinclude depend

help:
	# make clean      : cleans all libs and main program
	# --------------------------------------------------
	# make debug      : set debug mode of build params
	# make release    : set release mode of build params
	# make debug_dp   : set debug mode double prec of build params
	# make release_dp : set release mode double prec of build params
	# --------------------------------------------------
	# make memtrace   : set memtrace mode of build params
	# make run        : build and run program
	# make test       : optional impl. test run
	# --------------------------------------------------
	# make linux      : set linux as target system
	# make irix       : set irix n32 as target system
	# make irix_gcc   : set irix gcc as target system
	# make osx        : set Mac OS X as target system
	test -f compilerflags ||  echo make the 'platform' first
	test -f buildflags ||  echo make the 'build mode' first
	test -f compilerflags
	test -f buildflags

run:
	make
	./$(ENAME)

clean:
	touch xx.d
	rm -f *.d
	rm -f depend
	rm -f core
	rm -f so_locations
	rm -rf ii_files
	rm -rf $(FILES)
	rm -rf ./$(ENAME)
	rm -rf $(NAME)
	# ----------------- done cleaning up -------------------------------

cleandist:
	make clean
	rm -f buildflags
	rm -f compilerflags

debug:
	echo "DBG_EXT= -g" > buildflags
	echo "DEBUG =-DGZ_DEBUG" >> buildflags
	echo "#OPTI_EXT= -O2 $(OPTFLAGS)" >> buildflags
	echo "#MEM_DEBUG =-DGZ_MEM_DEBUG" >> buildflags
	echo "#DP_FLAG=-DGZ_DOUBLE_PRECISION" >> buildflags
	echo "#DP_EXT=-dp" >> buildflags
	make clean
	# ----------------- done debug setup --------------------------

debug_dp:
	echo "DBG_EXT= -g" > buildflags
	echo "DEBUG =-DGZ_DEBUG" >> buildflags
	echo "#OPTI_EXT= -O2 $(OPTFLAGS)" >> buildflags
	echo "#MEM_DEBUG =-DGZ_MEM_DEBUG" >> buildflags
	echo "DP_FLAG=-DGZ_DOUBLE_PRECISION" >> buildflags
	echo "DP_EXT=-dp" >> buildflags
	make clean
	# ----------------- done debug DP setup --------------------------


memtrace:
	echo "DBG_EXT= -g" > buildflags
	echo "DEBUG =-DGZ_DEBUG" >> buildflags
	echo "#OPTI_EXT= -O2 $(OPTFLAGS)" >> buildflags
	echo "MEM_DEBUG =-DGZ_MEM_DEBUG" >> buildflags
	make cleanall
	# ----------------- done memtrace debug setup --------------------------


release:
	echo "#DBG_EXT= -g" > buildflags
	echo "#DEBUG =-DGZ_DEBUG" >> buildflags
	echo "OPTI_EXT= -O2 $(OPTFLAGS)" >> buildflags
	echo "#MEM_DEBUG =-DGZ_MEM_DEBUG" >> buildflags
	echo "#DP_FLAG=-DGZ_DOUBLE_PRECISION" >> buildflags
	echo "#DP_EXT=-dp" >> buildflags
	make clean
	# ----------------- done release setup --------------------------


release_dp:
	echo "#DBG_EXT= -g" > buildflags
	echo "#DEBUG =-DGZ_DEBUG" >> buildflags
	echo "OPTI_EXT= -O2 $(OPTFLAGS)" >> buildflags
	echo "#MEM_DEBUG =-DGZ_MEM_DEBUG" >> buildflags
	echo "DP_FLAG=-DGZ_DOUBLE_PRECISION" >> buildflags
	echo "DP_EXT=-dp" >> buildflags
	make clean
	# ----------------- done release DP setup --------------------------

irix_gcc:
	echo "LINKOPTS =" > compilerflags
	echo "CXXOPTS =  -MD -DGZ_SGI -fno-rtti " >> compilerflags
	echo "DEPUPDATE = cat *.d > depend" >> compilerflags
	echo "OPENGLLIBS =  -lGL " >> compilerflags
	echo "EXT_LIBS =  -lX11 -lXm -lXt -ldl -lm -lpthread " >> compilerflags
	echo "OPTFLAGS = " >> compilerflags
	echo "PLUGIN = -shared" >> compilerflags
	echo "SHARED = -shared" >> compilerflags
	echo "RANLIB = echo" >> compilerflags >> compilerflags"
	echo "SO_EXT = .so" >> compilerflags
	echo "PLATFORM = irix_gcc" >> compilerflags
	echo "EDIR = ." >> compilerflags
	# ----------------- done irix GCC setup --------------------------

irix:
	echo "LINKOPTS = -n32 -L/usr/lib32/internal " > compilerflags
	echo "CXX = CC" >> compilerflags
	echo "CXXOPTS =  -DGZ_SGI -MDupdate depend -n32 -woff 1209,1682,3180,1314" >> compilerflags
	echo "DEPUPDATE = " >> compilerflags
	echo "OPENGLLIBS =  -lGL " >> compilerflags
	echo "EXT_LIBS = -lX11 -lXm -lXt -ldl -lm -lpthread " >> compilerflags
	echo "OPTFLAGS = -OPT:Olimit=15000 " >> compilerflags
	echo "PLUGIN = -shared" >> compilerflags
	echo "SHARED = -shared" >> compilerflags
	echo "RANLIB = echo" >> compilerflags
	echo "SO_EXT = .so" >> compilerflags
	echo "PLATFORM = irix" >> compilerflags
	echo "EDIR = ." >> compilerflags
	# ----------------- done irix setup --------------------------



linux:
	echo "LINKOPTS = " > compilerflags
	echo "CXXOPTS =  -MD -DGZ_LINUX -fno-rtti " >> compilerflags
	echo "DEPUPDATE = cat *.d > depend" >> compilerflags
	echo "OPTFLAGS = " >> compilerflags
	echo "EXT_LIBS = -lpthread -lm -ldl -L/usr/X11R6/lib -lX11 -lXext -lXmu -lXt -lXi -lSM " >> compilerflags
	echo "SHARED = -shared" >> compilerflags
	echo "PLUGIN = -shared" >> compilerflags
	echo "RANLIB = echo" >> compilerflags
	echo "SO_EXT = .so" >> compilerflags
	echo "PLATFORM = linux" >> compilerflags
	echo "EDIR = ." >> compilerflags
	# ----------------- done linux setup --------------------------

osx:
	echo "LINKOPTS = " > compilerflags
	echo "CXXOPTS =  -MD -DGZ_MAC -pipe -fPIC -fno-rtti -I/" >> compilerflags
	echo "CXX = c++" >> compilerflags
	echo "DEPUPDATE = cat *.d > depend" >> compilerflags
	echo "OPTFLAGS = " >> compilerflags
	echo "EXT_LIBS =   -framework Carbon -lm" >> compilerflags
	echo "RANLIB = ranlib" >> compilerflags
	echo "SHARED = -dynamiclib" >> compilerflags
	echo "PLUGIN = -bundle" >> compilerflags
	echo "SO_EXT = .dylib" >> compilerflags
	echo "PLATFORM = osx" >> compilerflags
	echo "EDIR = $(NAME)/Contents/MacOS" >> compilerflags
	# ----------------- done Mac OSX setup --------------------------




	

	


	
