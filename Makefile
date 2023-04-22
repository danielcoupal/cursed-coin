IDIR =include termbox2 
CC=g++
CFLAGS=$(patsubst %,-I%,$(IDIR))

ODIR=build/obj
LDIR =termbox2

LIBS=-lm -ltermbox2

vpath %.h $(IDIR)
DEPS = logging.h termbox2.h

_OBJ = main.o Ring.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

vpath %.c src

build_tree:
	mkdir -p build/obj

libtermbox2.a:
	$(MAKE) -Ctermbox2 lib

$(ODIR)/%.o: src/%.cpp $(DEPS) libtermbox2.a build_tree
	$(CC) -c -o $@ $< $(CFLAGS)

cursed-coin: $(OBJ)
	$(CC) -o build/$@ $^ $(CFLAGS) -L$(LDIR) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~

