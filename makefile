# Makefile
program = alcon2009
objs = main.o evaluation.o pnm.o my_alg.o alg_lvl1.o alg_lvl2.o alg_lvl3.o
CC = gcc
CFLAGS = -Wall -ansi -O2
#CFLAGS = -Wall -O2
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

alg_lvl1.o: alg_lvl1.c
	$(CC) $(CFLAGS) -c alg_lvl1.c

alg_lvl2.o: alg_lvl2.c
	$(CC) $(CFLAGS) -c alg_lvl2.c

alg_lvl3.o: alg_lvl3.c
	$(CC) $(CFLAGS) -c alg_lvl3.c

.PHONY: clean
clean:
	$(RM) $(objs) $(program)
