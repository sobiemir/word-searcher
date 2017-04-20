IDIR=inc
CC=g++
CFLAGS=-g -Wall -lncurses -lpthread
ODIR=obj
LIBS=-lncurses -lpthread
MKDIR=mkdir -p

DEPS=inc/configuration.hpp inc/interface.hpp inc/control.hpp inc/panel.hpp inc/searcher.hpp inc/textbox.hpp

_OBJS=main.o interface.o panel.o searcher.o textbox.o
OBJS=$(patsubst %,$(ODIR)/%,$(_OBJS))

.PHONY: all
.PHONY: clean
.PHONY: directories

$(ODIR)/%.o: src/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wordsearcher: obj/main.o $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

directories:
	$(MKDIR) $(ODIR)

all: directories wordsearcher

clean:
	rm -f $(ODIR)/*.o $(ODIR)/src/*.o
