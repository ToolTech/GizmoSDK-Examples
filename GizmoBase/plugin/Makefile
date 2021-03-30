sinclude  $(GIZMOSDK)/build/buildflags
sinclude  $(GIZMOSDK)/build/compilerflags
sinclude  $(GIZMOSDK)/build/libraryflags

FILES = plugtest.o  

TARGET = plugtest

include $(GIZMOSDK)/build/Makefile.inc

all : 
	make $(TARGET)
	make -f Makefile.plugin plugin$(SO_EXT)

cleanall:
	make clean
	make -f Makefile.plugin clean

