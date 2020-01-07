CC=g++
FLAGS= -std=c++11 -o

all:
	$(CC) bktprim.cpp $(FLAGS) bktprim
	./bktprim