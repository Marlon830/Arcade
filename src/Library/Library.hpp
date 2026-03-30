/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Library.hpp
*/

#pragma once

#include "DLLoader.hpp"
#include "IArcade.hpp"

namespace Arcade
{
    template <typename T>
    class Library
    {
    public:
        Library(const std::string &path, IArcade &arcade) :
            _loader(path)
        {
            this->_instance = this->_loader.getInstance(arcade);
        }

        ~Library()
        {
            this->_loader.deleteInstance(this->_instance);
        }

        T *getInstance() const
        {
            return this->_instance;
        }

        T *operator->() const
        {
            return this->_instance;
        }

        T &operator*() const
        {
            return *this->_instance;
        }

    private:
        DLLoader<T> _loader;
        T *_instance = nullptr;
    };
}
