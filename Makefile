CC = g++

mds: main.o mds.o
				$(CC) -o mds main.o mds.o -lsfml-graphics -lsfml-window -lsfml-system

main: main.cpp
					$(CC) -c main.cpp

mdscpp: mds.cpp mds.hpp
					$(CC) -c mds.cpp

clean:
				$(RM) mds *.o *~
