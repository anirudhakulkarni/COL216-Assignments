CC = g++
CFLAGS = -g -Wall

build:
	$(CC) RegisterFile.cpp MemoryUnit.cpp simulator.cpp -o sim $(CFLAGS)

run:
	./sim >output.txt

all : build run
