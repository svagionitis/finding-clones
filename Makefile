# Makefile
program = alcon2009
objs = main.o evaluation.o pnm.o my_alg.o alg1.o alg2.o alg3.o
CC = gcc
#CFLAGS = -Wall -ansi -O2
CFLAGS = -Wall -g -ggdb -ansi -O2
LDFLAGS = -lm
RM = rm -f

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

alg1.o: alg1.c
	$(CC) $(CFLAGS) -c alg1.c

alg2.o: alg2.c
	$(CC) $(CFLAGS) -c alg2.c

alg3.o: alg3.c
	$(CC) $(CFLAGS) -c alg3.c

.PHONY: clean
clean:
	$(RM) $(objs) $(program)
