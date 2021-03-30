
sinclude buildflags
sinclude compilerflags

default:
	make all

NAME = moduleTest
FILES = moduleTest.o  

sinclude ../Makefile.inc

all:
	make $(ENAME)
	make -f Makefile.plugin

cleanall:
	make clean
	make -f Makefile.plugin clean

test:
	make
	$(ENAME) module1

