/*
** EPITECH PROJECT, 2024
** B-PDG-300-MPL-3-1-PDGD11-marlon.pegahi
** File description:
** DirectoryTool.cpp
*/

#include "DirectoryTool.hpp"
#include <dirent.h>

DirectoryTool::DirectoryTool(const std::string &path)
{
    this->open(path);
}

DirectoryTool::~DirectoryTool()
{
    if (this->_directory) {
        closedir(this->_directory);
    }
}

bool DirectoryTool::open(const std::string &path)
{
    if (this->_directory) {
        if (closedir(this->_directory) == -1) {
            throw DirectoryTool::DirectoryToolError(DirectoryTool::DirectoryToolError::CLOSEDIR_ERROR);
        }
    }
    this->_directory = opendir(path.c_str());
    if (!(this->_directory)) {
        throw DirectoryTool::DirectoryToolError(DirectoryTool::DirectoryToolError::OPENDIR_ERROR);
    }
    return true;
}

std::string DirectoryTool::get()
{
    if (this->_directory) {
        struct dirent *actual = readdir(this->_directory);
        if (!actual)
            return "";
        return actual->d_name;
    }
    return "";
}
