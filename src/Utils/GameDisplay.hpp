/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** GameDisplay.hpp
*/

#pragma once

#include "Vector.hpp"
#include "TileMap.hpp"

namespace Arcade
{
    struct Sprite
    {
        std::string name;
        Vector2f pos;
        double rotation;
        bool xFlip;
        bool yFlip;
        bool visible = true;
    };

    struct GameDisplay
    {
        TileMap tiles;
        std::vector<Sprite> sprites;
        std::string music;
    };
}
