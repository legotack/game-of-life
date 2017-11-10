CC = g++
DEBUG = -g
INCLUDE = /usr/local/boost_1_61_0
CFLAGS = -Wall -I $(INCLUDE) -std=c++11 -c $(DEBUG)
LFLAGS = -Wall -I $(INCLUDE) -std=c++11 $(DEBUG)
EXE = game-of-life

$(EXE): main.o lifeGrid.o
	$(CC) $(LFLAGS) main.o lifeGrid.o -o game-of-life

main.o: main.cpp lifeGrid.h
	$(CC) $(CFLAGS) main.cpp

lifeGrid.o: lifeGrid.h lifeGrid.cpp
	$(CC) $(CFLAGS) lifeGrid.cpp

clean:
	\rm *.o *~ $(EXE)