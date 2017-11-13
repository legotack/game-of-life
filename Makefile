CC = g++
DEBUG = -g
INCLUDE = -I/usr/local/boost_1_61_0
SDL = -F/Library/Frameworks -framework SDL2
CFLAGS = -Wall $(INCLUDE) $(SDL) -std=c++11 -c $(DEBUG)
LFLAGS = -Wall $(INCLUDE) $(SDL) -std=c++11 $(DEBUG)
EXE = game-of-life

$(EXE): main.o lifeController.o lifeGrid.o lifeGraphics.o lifeMenu.o
	$(CC) $(LFLAGS) main.o lifeController.o lifeGrid.o lifeGraphics.o lifeMenu.o -o game-of-life

main.o: main.cpp lifeController.h lifeGraphics.h lifeSamples.h
	$(CC) $(CFLAGS) main.cpp

lifeController.o: lifeController.h lifeController.cpp lifeGrid.h
	$(CC) $(CFLAGS) lifeController.cpp

lifeGrid.o: lifeGrid.h lifeGrid.cpp
	$(CC) $(CFLAGS) lifeGrid.cpp

lifeGraphics.o: lifeGraphics.h lifeGraphics.cpp lifeGrid.h lifeMenu.h
	$(CC) $(CFLAGS) lifeGraphics.cpp

lifeMenu.o: lifeMenu.h lifeMenu.cpp
	$(CC) $(CFLAGS) lifeMenu.cpp

clean:
	\rm *.o *~ $(EXE)