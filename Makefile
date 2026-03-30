##
## EPITECH PROJECT, 2024
## B-OOP-400-MPL-4-1-arcade-marlon.pegahi
## File description:
## Makefile
##

SRC = src/Main.cpp \
		src/DirectoryTool/DirectoryTool.cpp \
		src/Arcade/Arcade.cpp \
		src/TileMap/TileMap.cpp

OBJ = $(SRC:.cpp=.o)

CXXFLAGS = -std=c++20 -W -Wall -Wextra -Werror -I./src/Interface -I./src/Utils -I./src/Arcade -I./src/DirectoryTool -I./src/DLLoader -I./src/Library -I./src/TileMap

TARGET = arcade

all: core games graphicals

clean:
	$(RM) $(OBJ)
	make clean -C ./lib/games
	make clean -C ./lib/library

fclean: clean
	$(RM) $(TARGET)
	make fclean -C ./lib/games
	make fclean -C ./lib/library

re: fclean all

core: $(OBJ)
	g++ -rdynamic -o $(TARGET) $(OBJ)

games:
	make -C ./lib/games

graphicals: 
	make -C ./lib/library
