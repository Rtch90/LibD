CC = g++
CFLAGS = -ansi -Wall -g
LDADD = -lGL -lGLU  -lglut -lSDL -lSDL_image -lSDL_gfx -lSDL_ttf -ltinyxml
LDADDSTATIC = -Wl,-Bstatic -lSDL -lSDL_image -lopenal -lalut -L/usr/X11 -Wl,-Bdynamic -lasound -lartsc -lesd -lpulse -lpulse-simple -ldirectfb -lvga -laa -lcaca -ljpeg -ltiff -

.PHONY: default static all clean

default: all

all:
	$(MAKE) -C ../src/Main
	$(MAKE) -C ../src/Texture
	$(MAKE) -C ../src/Actor
	$(MAKE) -C ../src/Math
	
	$(CC) $(CFLAGS) -o LibD ../src/Main/main.cpp ../src/Main/*.o ../src/Texture/*.o \
	                ../src/Actor/*.o  ../src/Math/*.o $(LDADD)

static:
	@echo -e "\033[1;31mThis is an experimental build, if it does not work, don't complain...\033[0m"
	@sleep 1
	$(MAKE) -C ../src/Main/ ../src/Main/*.o ../src/Texture/*.o \
                  ../src/Actor/*.o ../src/Math/*.o

	$(CC) $(CFLAGS) -o build/LibD-static ../src/Main/main.cpp ../src/Main/*.o  \
	                  ../src/Texture/*.o ../src/Actor/*.o ../src/Math/*.o $(LDADDSTATIC)

clean:
	$(MAKE) -C ../src/Main/	       clean
	$(MAKE) -C ../src/Texture/     clean
	$(MAKE) -C ../src/Actor/       clean
	$(MAKE) -C ../src/Math/        clean
	rm -f LibD
