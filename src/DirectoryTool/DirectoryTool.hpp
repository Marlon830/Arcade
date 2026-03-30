/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** DirectoryTool.hpp
*/

#pragma once

#include <dirent.h>
#include <string>
#include <exception>

class DirectoryTool
{
    public:
        class DirectoryToolError : public std::exception
        {
        public:
            enum ErrorType
            {
                NO_ERROR,
                OPENDIR_ERROR,
                CLOSEDIR_ERROR
            };

            DirectoryToolError(ErrorType t)
            {
                type = t;
            };

            const char *what() const noexcept override
            {
                switch (type)
                {
                case NO_ERROR:
                    return "";
                case OPENDIR_ERROR:
                    return "Could not open directory";
                case CLOSEDIR_ERROR:
                    return "Could not close directory";
                default:
                    return "";
                }
            }
        private:
            ErrorType type = NO_ERROR;
        };
        // Ctor
        DirectoryTool();
        DirectoryTool(const std::string &path);

        // Dtor
        ~DirectoryTool();

        // Member functions
        bool open(const std::string &path);
        std::string get();

    private:
        // Properties
        DIR *_directory = nullptr;
};
