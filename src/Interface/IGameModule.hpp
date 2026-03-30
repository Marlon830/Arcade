/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** IGameModule.hpp
*/

#pragma once

#include "Event.hpp"
#include "GameDisplay.hpp"
#include "Assets.hpp"

namespace Arcade
{
    class IGameModule
    {
    public:
        virtual ~IGameModule() = default;
        virtual void initGame() = 0;
        virtual Assets &getAssets() = 0;
        virtual void onUpdate(double delta) = 0;
        virtual GameDisplay &getDisplay() = 0;
        virtual void onEvent(Event) = 0;
    };
}
