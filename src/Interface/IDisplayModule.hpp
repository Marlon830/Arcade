/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** IDisplayModule.hpp
*/

#pragma once

#include "GameDisplay.hpp"
#include "Event.hpp"
#include "Rect.hpp"

namespace Arcade
{
    template <typename TTexture>
    struct Tile
    {
        TTexture *texture;
        Arcade::Rect rect;
    };

    class IDisplayModule
    {
    public:
        virtual ~IDisplayModule() = default;
        virtual void display(const GameDisplay &display) = 0;
        virtual void loadTileSet(const TileSet &tileSet) = 0;
        virtual void loadMusic(const std::string &filename, const std::string &musicId) = 0;
        virtual void loadSound(const std::string &filename, const std::string &soundId) = 0;
        virtual void playMusic(const std::string &soundId, bool repeat) = 0;
        virtual void playSound(const std::string &id) = 0;
        virtual Event &pollEvent() = 0;
    };
}
