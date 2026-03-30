/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** TileMap.cpp
*/

#include "TileMap.hpp"

namespace Arcade
{
    TileMap::TileMap(Vector2i size)
    {
        this->_mapSize = size;
        this->_tiles.resize(size.y);
        for (auto &row : this->_tiles)
            row.resize(size.x);
    }

    const Vector2i &TileMap::getMapSize() const
    {
        return this->_mapSize;
    }

    const std::string &TileMap::getTile(int x, int y) const
    {
        if (x < 0 || x >= this->_mapSize.x || y < 0 || y >= this->_mapSize.y)
            throw Arcade::TileMap::TileMapError(Arcade::TileMap::TileMapError::INVALID_TILE);
        return this->_tiles[y][x];
    }

    void TileMap::setTile(int x, int y, const std::string &id)
    {
        if (x < 0 || x >= this->_mapSize.x || y < 0 || y >= this->_mapSize.y)
            throw Arcade::TileMap::TileMapError(Arcade::TileMap::TileMapError::INVALID_TILE);
        this->_tiles[y][x] = id;
    }

    void TileMap::drawText(int x, int y, const std::string &text, const CharTable &charTable)
    {
        for (std::size_t i = 0; i < text.size(); i++) {
            if (charTable.find(text[i]) == charTable.end())
                continue;
            this->setTile(x + i, y, charTable.at(text[i]));
        }
    }

    void TileMap::forEach(std::function<void(std::string &, int, int)> &&callback)
    {
        for (int y = 0; y < this->_mapSize.y; y++)
            for (int x = 0; x < this->_mapSize.x; x++)
                callback(this->_tiles[y][x], x, y);
    }
    void TileMap::forEach(std::function<void(std::string &, int, int)> &callback)
    {
        for (int y = 0; y < this->_mapSize.y; y++)
            for (int x = 0; x < this->_mapSize.x; x++)
                callback(this->_tiles[y][x], x, y);
    }
}
