# makefile for root dir
# auther: richard
# time:   2013.10.6
include makefile.begin
export ROOTDIRS = $(shell pwd)
TARGET_DIRS = 	capturer	\
				engine	\
				progress	\
				util	\
				dataHandler

EXE = netpop

FUNC = $(MAKE) -fPIC -o ./bin/$(EXE) ./obj/*.o -lpcap

include makefile.in

.PHONY:clean
clean:
	@-rm ./obj/*.o ./bin/$(EXE) -rvf

