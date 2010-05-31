# Makefile
program = alcon2009
objs = main.o evaluation.o pnm.o my_alg.o
CC = gcc
CFLAGS = -Wall -ansi -O2
LDFLAGS = -lm
RM = rm

$(program): $(objs)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(program) $(objs)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

evaluation.o: evaluation.c
	$(CC) $(CFLAGS) -c evaluation.c

pnm.o: pnm.c
	$(CC) $(CFLAGS) -c pnm.c

my_alg.o: my_alg.c
	$(CC) $(CFLAGS) -c my_alg.c

.PHONY: clean
clean:
	$(RM) $(objs)
