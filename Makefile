IDIR =include termbox2 
CC=gcc
CFLAGS=$(patsubst %,-I%,$(IDIR))

ODIR=build/obj
LDIR =termbox2

LIBS=-lm -ltermbox2

vpath %.h $(IDIR)
DEPS = logging.h termbox2.h

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

vpath %.c src

libtermbox2.a:
	$(MAKE) -Ctermbox2 lib

$(ODIR)/%.o: src/%.c $(DEPS)
	mkdir -p build/obj
	$(CC) -c -o $@ $< $(CFLAGS)

cursed-coin: $(OBJ)
	mkdir -p build
	$(CC) -o build/$@ $^ $(CFLAGS) -L$(LDIR) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

