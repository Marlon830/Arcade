/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** IArcade.hpp
*/

#pragma once

#include "IGameModule.hpp"
#include "IDisplayModule.hpp"
#include <optional>

namespace Arcade
{
    enum class LibraryType
    {
        NONE = 0,
        GAME,
        DISPLAY
    };

    class IArcade
    {
    public:
        virtual void setGame(const std::string &name) = 0;
        virtual void setDisplay(const std::string &name) = 0;
        virtual void setName(const std::string &name) = 0;
        virtual void setScore(const std::string &gameName, int score) = 0;
        virtual IGameModule &getGame(void) = 0;
        virtual IDisplayModule &getDisplay(void) = 0;
        virtual const std::string &getName(void) = 0;
        virtual std::optional<int> getScore(const std::string &gameName) = 0;
        virtual std::map<std::string, std::string> getGames(void) = 0;
        virtual std::map<std::string, std::string> getDisplays(void) = 0;
    };
}
