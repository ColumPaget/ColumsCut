PREFIX=/usr/local
FLAGS=-g -Wall
all: common.o ccut

common.o: common.c
	cc $(FLAGS) -c common.c

ccut: ccut.c common.o
	cc $(FLAGS) -occut ccut.c common.o

clean:
	@rm -f *.o ccut 

check:
	@./check.sh

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp ccut $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	cp ccut.1 $(DESTDIR)$(PREFIX)/share/man/man1

test:
	@./check.sh
