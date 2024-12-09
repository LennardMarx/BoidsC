#!/bin/bash

# # obj
# gcc -c src/main.c -o obj/main.o
# gcc -c src/ui.c -o obj/ui.o
# gcc -c src/event_handler.c -o obj/event_handler.o
# gcc -c src/boid.c -o obj/boid.o
#
# # program
# # -lm -> linking math library
# # -W... -> error checkers
# # -SDL2 -> visualisation API
# # -O1/2/3 -> optimization
# gcc obj/*.o -lm -lSDL2 -Wall -Werror -Wextra -O1 -o bin/boids
# gcc src/*.c -lm -lSDL2 -Wall -Werror -Wextra -O1 -o bin/boids
# gcc src/*.c -lm -lSDL2 -Wall -O1 -o bin/boids



# SRC_DIR="src"
# OBJ_DIR="obj"
# BIN_DIR="bin"
#
# for file in $SRC_DIR/*.c; do
#     gcc -c "$file" -O3 -o "$OBJ_DIR/$(basename "${file%.c}.o")"
# done
#
# # Link object files to create the final executable
# gcc $OBJ_DIR/*.o -lm -lSDL2 -Wall -Werror -Wextra -O3 -o $BIN_DIR/boids

source ~/Software/emsdk/emsdk_env.sh
emcc src/*.c -s WASM=1 -s USE_SDL=2 -s SDL2_IMAGE_FORMATS='["bmp"]' -s USE_SDL_TTF=2 --preload-file resources -O3 -o boids_c.js
