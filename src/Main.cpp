/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Main.cpp
*/

#include "Arcade.hpp"
#include <iostream>
#include <chrono>
#include <thread>

#define ELAPSED_DIV 1

int main(int argc, char **argv)
{
    try {
        if (argc != 2)
            throw Core::SetupError(Core::SetupError::ErrorType::INVALID_NB_ARG);
        Core arcade;
        Arcade::Event event;
        arcade.setDisplayModuleFromPath(argv[1]);
        arcade.setGame("Menu");
        arcade.loadGameModule();
        arcade.loadDisplayModule();
        arcade.getGame().initGame();
        arcade.reloadTileSetsAndMusics();
        auto startTime = std::chrono::high_resolution_clock::now();
        while (1) {
            while ((event = arcade.getDisplay().pollEvent()).type != Arcade::Event::Type::NONE) {
                if (event.type == Arcade::Event::Type::CLOSE)
                    return 0;
                arcade.handleCoreEvents(event);
                arcade.getGame().onEvent(event);
            }
            auto endTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            arcade.getGame().onUpdate((float) elapsed.count() / ELAPSED_DIV);
            startTime = std::chrono::high_resolution_clock::now();
            arcade.getDisplay().display(arcade.getGame().getDisplay());
            arcade.handleGameOrLibChange();
        }
    } catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return 0;
}
