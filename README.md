# Tetris

## Description
A tutorial I did a while back during class at Chalmers to learn some basic C++

## How to run
 - Compile with non-standard path for SFML
```
$ g++ -c main.cpp -I SFML-/include
```

 - You must link the compiled file to the SFML libraries and tell the linker where to find the SFML libraries in order to get the final executable
   - SFML is made of 5 modules (system, window, graphics, network and audio), and there is one library for each of them
```
$ g++ main.o -o tetris -L SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system
```

 - Execute the compiled file
   - You need to tell the dynamic linker where to find the SFML libraries first by specifying LD_LIBRARY_PATH
```
$ export LD_LIBRARY_PATH=SFML-2.5.1/lib && ./tetris
```
