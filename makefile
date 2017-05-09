CC=gcc
DFLAGS=-lm -lrt
CFLAGS=-Wall
EXEC=mesure

all: $(EXEC)

run: all
	python setup.py

$(EXEC): main.o mesure.o utils.o signal.o functions.o debug.o console.o sys_info.o
	$(CC) -o $@ $^ $(DFLAGS)

main.o: main.c mesure.h functions.h debug.h console.h sys_info.h
	$(CC) -o $@ -c $< $(CFLAGS)

mesure.o: mesure.c mesure.h utils.h
	$(CC) -o $@ -c $< $(CFLAGS)

utils.o: utils.c utils.h
	$(CC) -o $@ -c $< $(CFLAGS)

signal.o: signal.c signal.h
	$(CC) -o $@ -c $< $(CFLAGS)

functions.o: functions.c functions.h mesure.h
	$(CC) -o $@ -c $< $(CFLAGS)

debug.o: debug.c debug.h
	$(CC) -o $@ -c $< $(CFLAGS)

console.o: console.c console.h sys_info.h
	$(CC) -o $@ -c $< $(CFLAGS)

sys_info.o: sys_info.c sys_info.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	# solve "clock skew" problem (Windows / Linux)
	# find . -type f | xargs -n 5 touch

	rm -rf *.o *.pyc __pycache__ $(EXEC)

clearscr:
	clear

fresh: clean clearscr all
