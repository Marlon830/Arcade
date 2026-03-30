/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Sfml.hpp
*/

#pragma once

#include <iostream>
#include "IDisplayModule.hpp"
#include "IArcade.hpp"
#include "GameDisplay.hpp"
#include "TileMap.hpp"
#include "Event.hpp"
#include "Rect.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

class Sfml : public Arcade::IDisplayModule
{
public:
    Sfml(Arcade::IArcade *arcade);
    ~Sfml();
    void display(const Arcade::GameDisplay &display) override;
    void loadTileSet(const Arcade::TileSet &tileSet) override;
    void loadMusic(const std::string &filename, const std::string &musicId) override;
    void loadSound(const std::string &filename, const std::string &soundId) override;
    void playMusic(const std::string &musicId, bool repeat) override;
    void playSound(const std::string &soundId) override;
    Arcade::Event &pollEvent() override;

private:
    Arcade::IArcade *_arcade;
    std::map<std::string, Arcade::Tile<sf::Texture>> _tiles;
    std::map<std::string, std::shared_ptr<sf::Music>> _musics;
    std::map<std::string, std::shared_ptr<sf::Sound>> _sounds;
    std::map<std::string, std::shared_ptr<sf::SoundBuffer>> _soundsBuffer;
    std::string _currentMusic;
    Arcade::Event _event;

    sf::RenderWindow _window;
};
