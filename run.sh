#!/bin/bash

g++ -c main.cpp -I resources/SFML-2.5.1/include
g++ main.o -o tetris -L resources/SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system
export LD_LIBRARY_PATH=resources/SFML-2.5.1/lib && ./tetris

rm *.o && rm tetris
