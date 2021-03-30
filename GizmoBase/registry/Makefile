sinclude  $(GIZMOSDK)/build/buildflags
sinclude  $(GIZMOSDK)/build/compilerflags
sinclude  $(GIZMOSDK)/build/libraryflags

FILES = gzgetenv.o  

TARGET = gzgetenv

include $(GIZMOSDK)/build/Makefile.inc

all : 
	make $(TARGET)
	make -f Makefile.gzsetenv gzsetenv
