all: quake-omp

CC = gcc
CFLAGS = -O2 -fopenmp

quake-omp: quake-omp.c
	$(CC) $(CFLAGS) -o quake-omp $< -lm

clean:
	rm -f quake-omp
