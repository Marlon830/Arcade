##
## EPITECH PROJECT, 2024
## B-OOP-400-MPL-4-1-arcade-marlon.pegahi
## File description:
## Makefile
##

LIB = ../../arcade_sdl2.so

SRC = Sdl2.cpp

OBJ = $(SRC:.cpp=.o)

CXXFLAGS = -std=c++20 -fPIC -W -Wall -Wextra -Werror -I../../../src/Arcade -I../../../src/Utils -I../../../src/Interface -I../../../src/Library -I../../../src/DLLoader -I../../../src/TileMap -I./.

LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer

all: $(OBJ)
	g++ -shared -fPIC -o $(LIB) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(LIB)

re: fclean all
