/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** TileMap.hpp
*/

#pragma once

#include <exception>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include "Vector.hpp"

namespace Arcade
{
    struct TileSet
    {
        std::string texturePath;
        Vector2i tileSize;
        std::vector<std::string> tileNames;
    };

    using CharTable = std::map<char, std::string>;

    class TileMap
    {
    public:
        class TileMapError : public std::exception
        {
        public:
            enum ErrorType
            {
                NO_ERROR,
                INVALID_TILE,
            };

            TileMapError(ErrorType t)
            {
                type = t;
            };

            const char *what() const noexcept override
            {
                switch (type)
                {
                case NO_ERROR:
                    return "";
                case INVALID_TILE:
                    return "Invalid tile";
                default:
                    return "";
                }
            }
        private:
            ErrorType type = NO_ERROR;
        };
        TileMap(Vector2i size);
        ~TileMap() = default;
        const Vector2i &getMapSize() const;
        const std::string &getTile(int x, int y) const;
        void setTile(int x, int y, const std::string &id);
        void forEach(std::function<void(std::string &, int, int)> &&callback);
        void forEach(std::function<void(std::string &, int, int)> &callback);
        void drawText(int x, int y, const std::string &text, const CharTable &charTable);

    protected:
        Vector2i _mapSize;
        std::vector<std::vector<std::string>> _tiles;
    };
}
