/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Assets.hpp
*/

#pragma once

#include <string>
#include <map>
#include "TileMap.hpp"

namespace Arcade
{
    struct Assets {
        std::vector<TileSet> tileSets;
        std::map<std::string, std::string> musics;
        std::map<std::string, std::string> sounds;
    };
}
