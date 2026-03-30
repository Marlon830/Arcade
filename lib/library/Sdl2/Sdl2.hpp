/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Sdl2.hpp
*/

#pragma once

#include <iostream>
#include "IDisplayModule.hpp"
#include "IArcade.hpp"
#include "GameDisplay.hpp"
#include "TileMap.hpp"
#include "Event.hpp"
#include "Rect.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

class Sdl2 : public Arcade::IDisplayModule
{
public:
    Sdl2(Arcade::IArcade *arcade);
    ~Sdl2();
    void display(const Arcade::GameDisplay &display) override;
    void loadTileSet(const Arcade::TileSet &tileSet) override;
    void loadMusic(const std::string &filename, const std::string &musicId) override;
    void loadSound(const std::string &filename, const std::string &soundId) override;
    void playMusic(const std::string &musicId, bool repeat) override;
    void playSound(const std::string &soundId) override;
    Arcade::Event &pollEvent() override;

private:
    Arcade::IArcade *_arcade;
    std::map<std::string, Arcade::Tile<SDL_Texture>> _tiles;
    std::map<std::string, Mix_Music *> _musics;
    std::map<std::string, Mix_Chunk *> _sounds;
    std::string _currentMusic;
    Arcade::Event _event;

    SDL_Window *_window;
    SDL_Renderer *_renderer;
};
