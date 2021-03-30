sinclude buildflags
sinclude compilerflags

NAME = network

FILES = network.o  

default:
	make all

sinclude ../Makefile.inc

all:
	make $(ENAME)
	make -f Makefile.netsend

cleanall:
	make clean
	make -f Makefile.netsend clean


