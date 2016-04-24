FLAGS=-g -Wall
all: common.o ccut

common.o: common.c
	gcc $(FLAGS) -c common.c

ccut: ccut.c common.o
	gcc $(FLAGS) -occut ccut.c common.o

clean:
	@rm -f *.o ccut 

check:
	@./check.sh
