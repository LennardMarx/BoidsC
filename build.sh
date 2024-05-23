#!/bin/bash

# obj
gcc -c src/main.c -o obj/main.o
gcc -c src/ui.c -o obj/ui.o
gcc -c src/event_handler.c -o obj/event_handler.o
gcc -c src/boid.c -o obj/boid.o
# gcc -c src/projection_plane.c -o obj/projection_plane.o
# gcc -c src/projector.c -o obj/projector.o
# gcc -c src/matrices.c -o obj/matrices.o
# gcc -c src/player.c -o obj/player.o

# program
# -lm -> math stuff
# -W... -> error checkers
# -SDL2 -> visualisation API
# -O1/2/3 -> optimization
gcc obj/*.o -lm -lSDL2 -Wall -Werror -Wextra -O1 -o bin/boids
