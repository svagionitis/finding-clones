# Makefile
program = alcon2009
objs = main.o evaluation.o pnm.o my_alg.o alg1.o alg2.o alg3.o color_features.o texture_features.o morphological_features.o morphological_operations.o
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

color_features.o: color_features.c
	$(CC) $(CFLAGS) -c color_features.c

texture_features.o: texture_features.c
	$(CC) $(CFLAGS) -c texture_features.c

morphological_features.o: morphological_features.c
	$(CC) $(CFLAGS) -c morphological_features.c

morphological_operations.o: morphological_operations.c
	$(CC) $(CFLAGS) -c morphological_operations.c

.PHONY: clean
clean:
	$(RM) $(objs) $(program)
