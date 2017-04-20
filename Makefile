IDIR=inc
CC=g++
CFLAGS=-g -Wall -lncurses -lpthread
ODIR=obj
LIBS=-lncurses -lpthread

DEPS=inc/configuration.hpp inc/interface.hpp inc/panel.hpp inc/searcher.hpp inc/textbox.hpp

_OBJ=main.o src/interface.o src/panel.o src/searcher.o src/textbox.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wordsearcher: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/src/*.o
