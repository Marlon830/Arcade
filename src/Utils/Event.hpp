/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Event.hpp
*/

#pragma once

namespace Arcade
{
    struct Event
    {
        enum class Type
        {
            NONE,
            KEY_UP,
            KEY_DOWN,
            MOUSE_CLICK,
            MOUSE_MOVE,
            CLOSE
        } type;

        union Data {
            struct Keyboard {
                char key;
            } keyboard;

            struct MouseClick {
                bool left_click;
                bool right_click;
                bool middle_click;
                int x;
                int y;
            } mouse_click;

            struct MouseMove {
                int x;
                int y;
            } mouse_move;

        } data;
    };
}
