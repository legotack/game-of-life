CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
EXE = game-of-life

$(EXE): main.o lifeGrid.o
	$(CC) $(LFLAGS) main.o lifeGrid.o -o game-of-life

main.o: main.cpp lifeGrid.h
	$(CC) $(CFLAGS) main.cpp

lifeGrid.o: lifeGrid.h lifeGrid.cpp
	$(CC) $(CFLAGS) lifeGrid.cpp

clean:
	\rm *.o *~ $(EXE)