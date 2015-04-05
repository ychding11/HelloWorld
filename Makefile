#This is the test for makefile
#
$(warning Beginning the makefile..........)
$(warning default CC = $(CC))
$(warning default LD = $(LD))
$(warning default CURDIR = $(CURDIR))
$(warning default CPPFLAGS = $(CPPFLAGS))

  

#SRCS := $(foreach dir,$(SUBDIRS), $(wildcard $(dir)/*.cpp)) #*/
#LIB_SRCS := $(filter-out test.cpp,$(wildcard *.cpp))

SRCS     := $(wildcard *.cpp)
TARGETS  := $(subst .cpp,, $(SRCS))
TARGETS_OBJ := $(subst cpp,o, $(SRCS))

$(info SRCS = $(SRCS))
$(info TARGETS = $(TARGETS))
$(info TARGETS_OBJ = $(TARGETS_OBJ))

CC = g++
$(info CC = $(CC))

.PHONY : all
all : $(TARGETS)
	@echo "building default target [ $@ ]...."

# 
.PHONY : $(TARGETS)	
$(TARGETS): $(TARGETS_OBJ)  
	@echo "buildig the target [$@.o ===> $@ ]..."
	$(CC) -lc   $@.o -o $@

	
%.o : %.cpp
	@echo begin compiling [ $< ==> $@ ]
	$(CC) $(CPPFLAGS) -c  $< -o $@
	
.PHONY : clean
clean :
	@echo beging clean temporal files....   # @ avoid echo the cmd itself
	@-rm -rf *.o  		                # - tell make continue even if rm encounters errors	
	-rm -rf *~	
	@echo clean finished....                # @ avoid echo the cmd itself		
	
