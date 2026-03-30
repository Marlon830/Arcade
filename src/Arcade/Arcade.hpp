/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Arcade.hpp
*/

#pragma once

#include "IArcade.hpp"
#include "Library.hpp"
#include <exception>
#include <optional>

class Core : public Arcade::IArcade
{
public:
    class SetupError : public std::exception
    {
    public:
        enum ErrorType
        {
            NO_ERROR,
            INVALID_NB_ARG,
            LIB_NOT_FOUND,
            LIB_NOT_COMPATIBLE
        };

        SetupError(ErrorType t)
        {
            type = t;
        };

        const char *what() const noexcept override
        {
            switch (type)
            {
            case NO_ERROR:
                return "";
            case INVALID_NB_ARG:
                return "Invalid number of arguments";
            case LIB_NOT_FOUND:
                return "Library not found";
            case LIB_NOT_COMPATIBLE:
                return "Library not compatible";
            default:
                return "";
            }
        }
    private:
        ErrorType type = NO_ERROR;
    };
    Core();
    Core(std::string &menuPath);
    void setGame(const std::string &name) override;
    void setDisplay(const std::string &name) override;
    void setName(const std::string &name) override;
    void setScore(const std::string &gameName, int score) override;
    Arcade::IGameModule &getGame(void) override;
    Arcade::IDisplayModule &getDisplay(void) override;
    const std::string &getName(void) override;
    std::optional<int> getScore(const std::string &gameName) override;
    std::map<std::string, std::string> getGames(void) override;
    std::map<std::string, std::string> getDisplays(void) override;

    void setHasGameChanged(bool hasGameChanged);
    bool getHasGameChanged(void) const;
    const std::string &getCurrentGame(void) const;
    std::optional<Arcade::Library<Arcade::IGameModule>> &getGameModule();

    void setHasDisplayChanged(bool hasDisplayChanged);
    bool getHasDisplayChanged(void) const;
    const std::string &getCurrentDisplay(void) const;
    std::optional<Arcade::Library<Arcade::IDisplayModule>> &getDisplayModule();

    void changeLib(int inc);
    void changeGame(int inc);

    void setIndexActualLib(int index);
    void setIndexActualGame(int index);

    void setDisplayModuleFromPath(std::string pathToLib);

    void loadGameModule();
    void loadDisplayModule();

    void reloadTileSetsAndMusics();

    void handleCoreEvents(Arcade::Event event);

    void handleGameOrLibChange();

private:
    std::optional<Arcade::Library<Arcade::IGameModule>> _gameModule;
    std::optional<Arcade::Library<Arcade::IDisplayModule>> _displayModule;
    std::string _gameMenu;
    std::map<std::string, std::string> _games;
    std::map<std::string, std::string> _displays;
    std::string _name;
    std::optional<int> _score;
    bool _hasGameChanged = false;
    std::string _currentGame;
    bool _hasDisplayChanged = false;
    std::string _currentDisplay;
    int _indexActualLib = 0;
    int _indexActualGame = 0;
};
