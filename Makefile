                                
# _|_|_|  _|_|    _|_|_|  _|_|    
# _|    _|    _|  _|    _|    _|  
# _|    _|    _|  _|    _|    _|  
# _|    _|    _|  _|    _|    _|  
#                                 
# Matrix Multiplication Project
# Copyright (C) 2011-2012 Andras Joo
#
#==============================================================================
SHELL=/bin/bash
include Makevars

#mmga folder with the header/source files
MMINC=./src
MMSRC=./src
MMBIN=./bin

#originally it was a dll, switched to statically linked library
DLLFILE=libmmga.a
DLL=$(MMBIN)/$(DLLFILE)
DLLSRC=$(wildcard $(MMSRC)/m*.cpp)
DLLOBJ=$(subst $(MMSRC),$(MMBIN), $(DLLSRC:.cpp=.o))
DLLDEP=$(DLLOBJ:.o=.d)

#each t*.cpp is assumed to be a test file with its own main function
TESTSRC=$(wildcard $(MMSRC)/t*.cpp)
TESTOBJ=$(subst $(MMSRC), $(MMBIN), $(TESTSRC:.cpp=.o))
TESTBIN=$(TESTOBJ:.o=.bin)
TESTDEP=$(TESTOBJ:.o=.d)

#each cl* is assumed to be an opencl kernel
CLSRC=$(wildcard $(MMSRC)/cl*.cpp)

CXX=g++
CXXFLAGS=-std=c++0x -g3 -O3 -Wall  -mtune=native
INC=-I$(MMINC) -I$(GAINC) -I$(ARMAINC) -I$(OPENCLINC) 
LIB=-lmmga -lga -lm -larmadillo -lpthread -lOpenCL -lrt
LIBPATH=-L$(GALIB) -L$(MMBIN)
LDFLAGS=

YELLOW=\e[00;33m
GREEN=\e[00;32m
BLUE=\e[0;34m
RESET=\e[00m

#==============================================================================

#in order to print variables we need phony targets
#.PHONY: DEBUG
#DEBUG:
#	@echo $(TESTSRC)
#	@echo $(DLL)

all: $(DLL) $(TESTBIN)

$(DLL): $(DLLOBJ)
	@echo -e '$(GREEN)[Creating library $@]$(RESET)'
	@ar -cr -o $@ $^

#include dependencies
-include $(DLLDEP)

#stop make deleting the intermediate .o files
.SECONDARY:
$(MMBIN)/%.o : $(MMSRC)/%.cpp 
	@echo -e '$(YELLOW)[Compiling $<]$(RESET)'
	@$(CXX) -c $(CXXFLAGS) $(INC) -o $@ $< 	
	@$(CXX) -MM $(CXXFLAGS) $(INC) $(MMSRC)/$*.cpp > $(MMBIN)/$*.d	

$(MMBIN)/%.bin : $(MMBIN)/%.o $(DLL)
	@echo -e '$(GREEN)[Linking $@]$(RESET)'
	@$(CXX) $(LDFLAGS) $< $(LIBPATH) $(LIB) -o $@ 


clean:
	@rm -f $(DLL) $(DLLOBJ) $(DLLDEP) $(TESTOBJ) $(TESTDEP) $(TESTBIN)



