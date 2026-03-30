##
## EPITECH PROJECT, 2024
## B-OOP-400-MPL-4-1-arcade-marlon.pegahi
## File description:
## Makefile
##

LIB = ../../arcade_ncurses.so

SRC = Ncurses.cpp

OBJ = $(SRC:.cpp=.o)

CXXFLAGS = -std=c++20 -fPIC -W -Wall -Wextra -Werror -I../../../src/Arcade -I../../../src/Utils -I../../../src/Interface -I../../../src/Library -I../../../src/DLLoader -I../../../src/TileMap -I./.

LDFLAGS = -lncurses

all: $(OBJ)
	g++ -shared -fPIC -o $(LIB) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(LIB)

re: fclean all
