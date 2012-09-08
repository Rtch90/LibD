CC=cc
CFLAGS=-O -I../..i -c
#LDFLAGS += -lz

UNZ_OBJS = miniunz.o unzip.o ioapi.o #libz.a
ZIP_OBJS = minizip.o zip.o   ioapi.o #libz.a

.c.o:
	$(CC) -c $(CFLAGS) $*.c

all: miniunz minizip

miniunz:  $(UNZ_OBJS)
	#$(CC) $(CFLAGS) $(UNZ_OBJS) #$(LDFLAGS)

minizip:  $(ZIP_OBJS)
	$(AR) -cvq ../minizip.a $(UNZ_OBJS) $(ZIP_OBJS)
	#$(CC) $(CFLAGS) $(ZIP_OBJS) #$(LDFLAGS)

test:	miniunz minizip
	./minizip test readme.txt
	./miniunz -l test.zip
	mv readme.txt readme.old
	./miniunz test.zip

clean:
	/bin/rm -f *.o minizip miniunz
