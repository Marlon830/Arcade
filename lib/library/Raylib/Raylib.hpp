/*
** EPITECH PROJECT, 2023
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Raylib.hpp
*/
#pragma once

#include <iostream>
#include "IDisplayModule.hpp"
#include "IArcade.hpp"
#include "GameDisplay.hpp"
#include "TileMap.hpp"
#include "Event.hpp"
#include "Rect.hpp"
#include "raylib.h"

class Raylib : public Arcade::IDisplayModule
{
public:
    Raylib(Arcade::IArcade *arcade);
    ~Raylib();
    void display(const Arcade::GameDisplay &display) override;
    void loadTileSet(const Arcade::TileSet &tileSet) override;
    void loadMusic(const std::string &filename, const std::string &musicId) override;
    void loadSound(const std::string &filename, const std::string &soundId) override;
    void playMusic(const std::string &musicId, bool repeat) override;
    void playSound(const std::string &soundId) override;
    Arcade::Event &pollEvent() override;

private:
    Arcade::IArcade *_arcade;
    // std::vector<TTexture> _textures;
    std::map<std::string, Arcade::Tile<Texture2D>> _tiles;
    std::map<std::string, Music *> _musics;
    std::map<std::string, Sound *> _sounds;
    std::string _currentMusic;
    Arcade::Event _event;
    int _lastKeyPressed = -1;

    // TODO: remove window initialization in the constructor
};
