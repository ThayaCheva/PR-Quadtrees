# define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# define libraries to be linked (for example -lm)
LDLIBS = 

# define sets of header source files and object files
SRC = main.c data.c list.c quadtree.c
# OBJ is the same as SRC, just replace .c with .h
OBJ = $(SRC:.c=.o)
 
# define the executable name(s)
EXE1 = dict3

# define the executable name(s)
EXE2 = dict4

all: $(EXE1) $(EXE2)

# the first target:
$(EXE1): $(OBJ) 
	$(CC) $(CFLAGS) -o dict3 $(OBJ) $(LDLIBS)

# the first target:
$(EXE2): $(OBJ) 
	$(CC) $(CFLAGS) -o dict4 $(OBJ) $(LDLIBS)


clean:
	rm -f $(OBJ) $(EXE)
