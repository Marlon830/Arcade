/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Ncurses.hpp
*/

#pragma once

#include <iostream>
#include "IDisplayModule.hpp"
#include "IArcade.hpp"
#include "GameDisplay.hpp"
#include "TileMap.hpp"
#include "Event.hpp"
#include "Rect.hpp"
#include <ncurses.h>
#include <chrono>
#include <memory>

enum NcursesColor
{
    NCURSES_BLACK,
    NCURSES_RED,
    NCURSES_GREEN,
    NCURSES_YELLOW,
    NCURSES_BLUE,
    NCURSES_MAGENTA,
    NCURSES_CYAN,
    NCURSES_WHITE
};

struct NcursesTexture
{
    NcursesColor color;
    std::string textToDisplay;
};

class Ncurses : public Arcade::IDisplayModule
{
public:
    Ncurses(Arcade::IArcade *arcade);
    void display(const Arcade::GameDisplay &display) override;
    void loadTileSet(const Arcade::TileSet &tileSet) override;
    void loadMusic(const std::string &filename, const std::string &musicId) override;
    void loadSound(const std::string &filename, const std::string &soundId) override;
    void playMusic(const std::string &soundId, bool repeat) override;
    void playSound(const std::string &id) override;
    Arcade::Event &pollEvent() override;

private:
    Arcade::IArcade *_arcade;
    // std::vector<TTexture> _textures;
    std::vector<std::shared_ptr<NcursesTexture>> _textures;
    std::map<std::string, Arcade::Tile<NcursesTexture>> _tiles;
    // std::map<std::string, TMusic> _musics;
    // std::map<std::string, TSound> _sounds;
    std::string _currentMusic;
    std::map<std::string, NcursesColor> _colorList;
    Arcade::Event _event;
    WINDOW *_window;
    int _textureIndex = 0;
};
