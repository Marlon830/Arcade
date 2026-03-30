/*
** EPITECH PROJECT, 2024
** Visual Studio Live Share (Espace de travail)
** File description:
** Menu.hpp
*/

#pragma once

#include "Library.hpp"
#include "IArcade.hpp"
#include "IGameModule.hpp"
#include <iostream>
#include "TileMap.hpp"

class Menu : public Arcade::IGameModule {
public:
    Menu(Arcade::IArcade *arcade);
    void initGame();
    void onUpdate(double delta);
    void onEvent(Arcade::Event ev);

    Arcade::Assets &getAssets() { 
        return this->_assets;
    };

    Arcade::GameDisplay &getDisplay() {
        return this->_gameDisplay;
    };
    
private:
    Arcade::IArcade *arcade;
    std::string _name;
    int _menuIndex = 0;
    int _menuColumn = 0;
    std::string _currentGame;
    std::string _currentDisplay;
    int _nbGame = 0;
    int _nbDisplay = 0;
    std::map<std::string, std::string> _games;
    std::map<std::string, std::string> _displays;
    Arcade::Assets _assets;
    Arcade::Vector2i _mapSize;
    Arcade::CharTable _charTable;

    // HighScores
    std::map<std::string, int> _highScoresMap;

    Arcade::GameDisplay _gameDisplay{Arcade::TileMap(Arcade::Vector2i {0, 0}), {}, ""};
};
