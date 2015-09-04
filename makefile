CC = gcc
CFLAGS = 
DEPS = 
LDFLAGS= -lalleg -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput -lddraw -ldxguid -lwinmm -ldsound -static-libgcc
OBJECTS = maped.o
output = maped.exe
prefix = c:\sprocket

maped.exe : $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(output) $(LDFLAGS)

%.o : %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<
	
install : maped.exe
	install -d $(prefix)
	install maped.exe $(prefix)
.PHONY : install
