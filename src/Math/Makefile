CC	= g++
CFLAGS	= -ansi -Wall -g
LDADD	= -lGL -lGLU -lSDL -lSDL_image

objects = FPS.o Timer.o Vec2.o \


.PHONY: default all clean

default: all

%.cpp: %.h

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(objects)

clean:
	rm -f $(objects)
