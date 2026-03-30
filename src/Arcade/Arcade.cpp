/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Arcade.cpp
*/

#include "Arcade.hpp"
#include "DirectoryTool.hpp"
#include <iostream>

Core::Core()
{
    this->_gameMenu = "lib/arcade_menu.so";
    const std::string libPath("./lib/");
    DirectoryTool dir(libPath);
    std::string filename;


    while ((filename = dir.get()) != "") {
        if (filename.find(".so") != std::string::npos && filename.find("arcade") != std::string::npos) {
            Arcade::DLLoader<Arcade::IGameModule> loader(libPath + filename);
            if (loader.getLibraryType() == Arcade::LibraryType::GAME) {
                this->_games[loader.getLibraryName()] = libPath + filename;
            } else {
                Arcade::DLLoader<Arcade::IDisplayModule> loader(libPath + filename);
                this->_displays[loader.getLibraryName()] = libPath + filename;
            }
        }
    }
}

Core::Core(std::string &menuPath)
{
    this->_gameMenu = menuPath;
    const std::string libPath("./lib/");
    DirectoryTool dir(libPath);
    std::string filename;

    while ((filename = dir.get()) != "") {
        if (filename.find(".so") != std::string::npos && filename.find("arcade") != std::string::npos) {
            Arcade::DLLoader<Arcade::IGameModule> loader(libPath + filename);
            if (loader.getLibraryType() == Arcade::LibraryType::GAME) {
                this->_games[loader.getLibraryName()] = libPath + filename;
            } else {
                Arcade::DLLoader<Arcade::IDisplayModule> loader(libPath + filename);
                this->_displays[loader.getLibraryName()] = libPath + filename;
            }
        }
    }
}

void Core::setGame(const std::string &name)
{
    this->_currentGame = name;
    this->_hasGameChanged = true;

    int i = 0;
    for (auto &game : this->_games) {
        if (game.first == name) {
            this->_indexActualGame = i;
        }
        i++;
    }
}

void Core::setDisplay(const std::string &name)
{
    this->_currentDisplay = name;
    this->_hasDisplayChanged = true;
}

void Core::setName(const std::string &name)
{
    this->_name = name;
}

void Core::setScore([[maybe_unused]]const std::string &gameName, int score)
{
    this->_score = score;
}

Arcade::IGameModule &Core::getGame(void)
{
    return *(*(this->_gameModule));
}

Arcade::IDisplayModule &Core::getDisplay(void)
{
    return *(*(this->_displayModule));
}

const std::string &Core::getName(void)
{
    return this->_name;
}

std::optional<int> Core::getScore(const std::string &gameName)
{
    (void) gameName;
    return this->_score;
}

std::map<std::string, std::string> Core::getGames(void)
{
    return this->_games;
}

std::map<std::string, std::string> Core::getDisplays(void)
{
    return this->_displays;
}

bool Core::getHasGameChanged(void) const
{
    return this->_hasGameChanged;
}

const std::string &Core::getCurrentGame(void) const
{
    return this->_currentGame;
}

std::optional<Arcade::Library<Arcade::IGameModule>> &Core::getGameModule()
{
    return this->_gameModule;
}

void Core::setHasGameChanged(bool hasGameChanged)
{
    this->_hasGameChanged = hasGameChanged;
}

void Core::setHasDisplayChanged(bool hasDisplayChanged)
{
    this->_hasDisplayChanged = hasDisplayChanged;
}

bool Core::getHasDisplayChanged(void) const
{
    return this->_hasDisplayChanged;
}

const std::string &Core::getCurrentDisplay(void) const
{
    return this->_currentDisplay;
}

std::optional<Arcade::Library<Arcade::IDisplayModule>> &Core::getDisplayModule()
{
    return this->_displayModule;
}

void Core::changeLib(int inc)
{
    int i = 0;

    this->_indexActualLib += inc;
    if (this->_indexActualLib >= (int) this->_displays.size())
        this->_indexActualLib = 0;
    if (this->_indexActualLib < 0)
        this->_indexActualLib = this->_displays.size() - 1;
    for (auto display : this->_displays) {
        if (i == this->_indexActualLib) {
            this->_currentDisplay = display.first;
            this->_hasDisplayChanged = true;
        }
        i++;
    }
    return;
}

void Core::changeGame(int inc)
{
    int i = 0;

    this->_indexActualGame += inc;
    if (this->_indexActualGame >= (int) this->_games.size())
        this->_indexActualGame = 0;
    if (this->_indexActualGame < 0)
        this->_indexActualGame = this->_games.size() - 1;
    for (auto game : this->_games) {
        if (i == this->_indexActualGame) {
            this->_currentGame = game.first;
            this->_hasGameChanged = true;
        }
        i++;
    }
    return;
}

void Core::setIndexActualLib(int index)
{
    this->_indexActualLib = index;
}

void Core::setIndexActualGame(int index)
{
    this->_indexActualGame = index;
}

void Core::setDisplayModuleFromPath(std::string pathToLib)
{
    Arcade::DLLoader<Arcade::IDisplayModule> loader(pathToLib);
    if (loader.getLibraryType() != Arcade::LibraryType::DISPLAY)
        throw Core::SetupError(Core::SetupError::ErrorType::LIB_NOT_COMPATIBLE);
    bool libExist = false;
    int i = 0;
    for (auto &display : this->_displays) {
        if (display.first == loader.getLibraryName()) {
            this->_indexActualLib = i;
            libExist = true;
        }
        i++;
    }
    if (!libExist)
        throw Core::SetupError(Core::SetupError::ErrorType::LIB_NOT_COMPATIBLE);
    this->setDisplay(loader.getLibraryName());
}

void Core::loadGameModule()
{
    if (this->_gameModule.has_value())
        this->_gameModule.reset();
    this->_gameModule.emplace(this->_games[this->_currentGame], *this);
    this->_hasGameChanged = false;
}

void Core::loadDisplayModule()
{
    if (this->_displayModule.has_value())
        this->_displayModule.reset();
    this->_displayModule.emplace(this->_displays[this->_currentDisplay], *this);
    this->_hasDisplayChanged = false;
}

void Core::reloadTileSetsAndMusics()
{
    std::vector<Arcade::TileSet> tileSets = this->getGame().getAssets().tileSets;
    for (std::size_t i = 0; i != tileSets.size(); i++) {
        this->getDisplay().loadTileSet(tileSets[i]);
    }
    for (auto &music : this->getGame().getAssets().musics) {
        this->getDisplay().loadMusic(music.second, music.first);
    }
    for (auto &sound : this->getGame().getAssets().sounds) {
        this->getDisplay().loadSound(sound.second, sound.first);
    }
}

void Core::handleCoreEvents(Arcade::Event event)
{
    if (event.type == Arcade::Event::Type::KEY_DOWN && event.data.keyboard.key == 'l')
        this->changeLib(-1);
    if (event.type == Arcade::Event::Type::KEY_DOWN && event.data.keyboard.key == 'm')
        this->changeLib(1);
    if (event.type == Arcade::Event::Type::KEY_DOWN && event.data.keyboard.key == 'o') {
        this->getGame().getDisplay().music = "";
        this->changeGame(-1);
    }
    if (event.type == Arcade::Event::Type::KEY_DOWN && event.data.keyboard.key == 'p') {
        this->getGame().getDisplay().music = "";
        this->changeGame(1);
    }
    if (event.type == Arcade::Event::Type::KEY_DOWN && event.data.keyboard.key == 'i')
        this->getGame().initGame();
    if (event.type == Arcade::Event::Type::KEY_DOWN && event.data.keyboard.key == 'k') {
        this->getGame().getDisplay().music = "";
        this->setGame("Menu");
    }
}

void Core::handleGameOrLibChange()
{
    if (this->getHasGameChanged()) {
        this->loadGameModule();
        this->getGame().initGame();
        this->reloadTileSetsAndMusics();
    }
    if (this->getHasDisplayChanged()) {
        this->loadDisplayModule();
        this->reloadTileSetsAndMusics();
    }   
}
