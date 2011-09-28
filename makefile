#!gmake 

#-----------------------------------------
#Basic Stuff -----------------------------
CC          = g++ 
cc          = gcc

#-----------------------------------------
#Optimization ----------------------------
OPT   = -O3

#-----------------------------------------
# X       --------------------------------
X_INC  =   -I/usr/X11R6/include -I/sw/include -I/usr/sww/include -I/usr/sww/pkg/Mesa/include -I./FreeImage/ -I./
X_LIB  =   -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib -L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -L./FreeImage/ -lfreeimage -L./

#-----------------------------------------

#-----------------------------------------

TARGETS = Scene

OBJECTS = Scene.o

#-----------------------------------------

LIBS = $(X_LIB)
INCS = $(X_INC)

CCOPTS = $(OPT) $(INCS)
LDOPTS = $(OPT) $(INCS) $(LIBS)

#-----------------------------------------
#-----------------------------------------

default: $(TARGETS)

clean:
	/bin/rm -f *.o $(TARGETS)

#-----------------------------------------
#-----------------------------------------

Scene: Scene.o
	$(CC) Scene.o $(LDOPTS) -o Scene 

Scene.o: Scene.cpp
	$(CC) Scene.cpp -c $(CCOPTS)


#export LD_LIBRARY_PATH="/usr/sww/pkg/Mesa/lib:/usr/lib:/usr/sww/lib"

