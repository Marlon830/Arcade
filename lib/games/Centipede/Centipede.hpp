/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Centipede.hpp
*/

#pragma once

#include "Library.hpp"
#include "IArcade.hpp"
#include "IGameModule.hpp"
#include <iostream>
#include "TileMap.hpp"
#include <cmath>

struct CentipedePart
{
    Arcade::Vector2i centipedeVelocity;
    Arcade::Vector2f pos;
    double rotate;
    bool isLinked;
    bool goDown;
    bool isDead;
};

class Centipede : public Arcade::IGameModule {
public:
    Centipede(Arcade::IArcade *arcade);
    void initGame();
    void onUpdate(double delta);
    void onEvent(Arcade::Event ev);
    void gameUpdate(double delta);
    void gameOverUpdate(double delta);
    void resetCentipede();
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

    // Player
    Arcade::Vector2f _playerPos;
    Arcade::Vector2f _playerVelocity;

    // Missile
    Arcade::Vector2f _missilePos;
    Arcade::Vector2i _missileVelocity;
    bool _missileExists;

    // Centipede
    std::vector<CentipedePart> _centipede;

    // Obstacles
    std::vector<std::vector<int>> _obstacles;

    // HighScores
    std::vector<int> _highScores;
    std::vector<std::string> _highScoresNames;
    
    // Game Variables
    int _menuSelection = 0;
    bool _playerAlive = true;
    float _updateTimer = 0.0;
    float _centipedCanMove = 0;
    float _timer = 0.0;
    int _score = 0;
    int _nbCentipede = 0;
};
