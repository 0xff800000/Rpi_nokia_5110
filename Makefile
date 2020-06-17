CC=gcc
LFLAGS=-lwiringPi 
CFLAGS=-std=c99 -D _DEFAULT_SOURCE -g

Nokia/Nokia.o: Nokia/Nokia.c Nokia/Nokia.h
	$(CC) -c $< -o $@ $(CFLAGS) $(LFLAGS)

main.o: main.c
	$(CC) -c $^ -o $@ $(CFLAGS) $(LFLAGS)

vcsprint.o: vcsprint.c
	$(CC) -c $^ -o $@ $(CFLAGS) $(LFLAGS)

gameOfLife.o: gameOfLife.c
	$(CC) -c $^ -o $@ $(CFLAGS) $(LFLAGS)

vcsprint: vcsprint.o Nokia/Nokia.o
	$(CC) $^ -o $@ $(LFLAGS)

lcd: main.o Nokia/Nokia.o
	$(CC) $^ -o $@ $(LFLAGS)

gol: gameOfLife.o Nokia/Nokia.o
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	rm -f *.o Nokia/*.o lcd vcsprint
