/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** DLLoader.hpp
*/

#pragma once

#include <string>
#include <dlfcn.h>
#include <stdexcept>
#include "IArcade.hpp"
#include <iostream>

enum class LibraryType;

namespace Arcade
{
    template <typename T>
    class DLLoader
    {
    public:
        class LoaderError : public std::exception
        {
        public:
            LoaderError(const std::string &error)
            {
                _error = error;
            }

            const char *what() const noexcept override
            {
                return _error.c_str();
            }

        private:
            std::string _error;
        };

        DLLoader(const std::string &path)
        {
            this->_handle = dlopen(path.c_str(), RTLD_LAZY);
            if (!this->_handle)
            {
                throw LoaderError(dlerror());
            }
            this->_path = path;
        }

        ~DLLoader()
        {
            if (this->_handle)
            {
                dlclose(this->_handle);
                this->_handle = nullptr;
            }
        }

        DLLoader(const DLLoader &) = default;

        DLLoader &operator=(const DLLoader &) = default;

        // Getters
        LibraryType getLibraryType()
        {
            dlerror(); // Clear any existing error
            LibraryType (*getType)() = (LibraryType(*)())dlsym(this->_handle, "getType");
            const char *dlsym_error = dlerror();
            if (dlsym_error)
            {
                throw LoaderError(dlsym_error);
            }
            return getType();
        }

        std::string getLibraryName()
        {
            dlerror(); // Clear any existing error
            std::string (*getName)() = (std::string(*)())dlsym(this->_handle, "getName");
            const char *dlsym_error = dlerror();
            if (dlsym_error)
            {
                throw LoaderError(dlsym_error);
            }
            return getName();
        }

        T *getInstance(IArcade &arcade)
        {
            dlerror(); // Clear any existing error
            T *(*getInstance)(IArcade &) = (T * (*)(IArcade &)) dlsym(this->_handle, "getInstance");
            const char *dlsym_error = dlerror();
            if (dlsym_error)
            {
                throw std::runtime_error(dlsym_error);
            }
            return getInstance(arcade);
        }

        void deleteInstance(T *instance)
        {
            dlerror(); // Clear any existing error
            void (*deleteInst)(T *) = (void (*)(T *))dlsym(this->_handle, "deleteInstance");
            const char *dlsym_error = dlerror();
            if (dlsym_error)
            {
                throw LoaderError(dlsym_error);
            }
            deleteInst(instance);
        }

    private:
        std::string _path;
        void *_handle = nullptr;
    };
}
