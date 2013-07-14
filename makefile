IDIR =include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lm -lbluetooth -lcwiid -lpthread

_DEPS = ball_plate.h manual_mode.h micro_maestro.h playsound.h wiimote_stuff.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o manual_mode.o micro_maestro.o playsound.o wiimote_stuff.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ball_plate: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	
startup:
	chmod +x scripts/ball_plate
	cp scripts/ball_plate /etc/init.d
	update-rc.d ball_plate defaults
	
not_startup:
	update-rc.d -f ball_plate remove
	rm /etc/init.d/ball_plate
