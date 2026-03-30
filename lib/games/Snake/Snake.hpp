/*
** EPITECH PROJECT, 2024
** Visual Studio Live Share (Espace de travail)
** File description:
** Snake.hpp
*/

#pragma once

#include "Library.hpp"
#include "IArcade.hpp"
#include "IGameModule.hpp"
#include <iostream>
#include "TileMap.hpp"
#include <cmath>

class Snake : public Arcade::IGameModule {
public:
    Snake(Arcade::IArcade *arcade);
    ~Snake();
    void initGame();
    void onUpdate(double delta);
    void onEvent(Arcade::Event ev);
    void gameUpdate(double delta);
    void gameOverUpdate(double delta);
    void updateHighScores();

    Arcade::Assets &getAssets() { 
        return this->_assets;
    };

    Arcade::GameDisplay &getDisplay() {
        return this->_gameDisplay;
    };
    
private:
    // IGameModule
    Arcade::IArcade *arcade;
    Arcade::Assets _assets;
    Arcade::GameDisplay _gameDisplay{Arcade::TileMap(Arcade::Vector2i {0, 0}), {}, ""};
    std::string name;
    
    // TileMap
    Arcade::Vector2i _mapSize;
    Arcade::CharTable _charTable;

    // Snake
    Arcade::Vector2f _snakeVelocity;
    std::vector<Arcade::Vector2f> _snakePos;
    std::vector<double> _snakeDir;
    std::vector<Arcade::Vector2f> _snakeVelocityList;

    // Apple
    Arcade::Vector2f _applePos;

    // HighScores
    std::vector<int> _highScores;
    std::vector<std::string> _highScoresNames;
    
    // Game Variables
    int _menuSelection = 0;
    bool _isAlive = true;
    float _updateTimer = 0.0;
    float _timer = 0.0;
    int _score = 0;
    bool _hasMoved = false;
};
