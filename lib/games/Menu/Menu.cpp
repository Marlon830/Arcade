/*
** EPITECH PROJECT, 2024
** Visual Studio Live Share (Espace de travail)
** File description:
** Menu.hpp
*/

#include "Menu.hpp"
#include <fstream>

#define DOWN_ARROW 'Q'
#define UP_ARROW 'R'
#define RIGHT_ARROW 'O'
#define LEFT_ARROW 'P'
#define TILE_SIZE 32

Menu::Menu(Arcade::IArcade *arcade)
{
    srand(time(NULL));
    this->arcade = arcade;
    this->_mapSize = {50, 50};
    this->_games = this->arcade->getGames();
    this->_nbGame = this->_games.size() - 1;
    this->_displays = this->arcade->getDisplays();
    this->_nbDisplay = this->_displays.size();
    this->_name = arcade->getName();

    std::vector<std::string> charNamesTemp;
    for (int i = 32; i != 127; i++) {
        charNamesTemp.push_back(std::string(1, (char)i));
        this->_charTable[(char)i] = std::string(1, (char)i);
    }
    this->_assets.tileSets.push_back({"assets/font/font_white.png", {TILE_SIZE, TILE_SIZE}, charNamesTemp});
    this->_assets.musics["menu_music"] = "assets/menu/music.ogg";
    this->_assets.sounds["menu_button_sound"] = "assets/menu/button_menu.ogg";

    int readScores = false;
    std::ifstream file("highScores.yaml");
    if (file.is_open()) {
        std::string line;
        std::string currentGame;
        std::string name;
        while (std::getline(file, line)) {
            if (line[0] == '#') {
                name = line.substr(1, line.size() - 1);
                readScores = true;
                continue;
            }
            if (line.empty() || !readScores) {
                continue;
            }
            if (readScores) {
                this->_highScoresMap[name] = std::stoi(line.substr(line.find(":") + 1, line.size() - (line.find(":") + 1)));
                readScores = false;
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to open file highScores.yaml" << std::endl;
    }
}

void Menu::initGame()
{
    this->_gameDisplay.music = "menu_music";
}

void Menu::onUpdate([[maybe_unused]]double delta)
{
    int i = 2;

    this->_gameDisplay.tiles = Arcade::TileMap(this->_mapSize);
    if (this->_menuIndex == -1) {
        this->_gameDisplay.tiles.drawText(8, 0, ">", this->_charTable);
    }
    this->_gameDisplay.tiles.drawText(10, 0, "Your name: " + this->_name, this->_charTable);
    for (const auto &pair : this->_games) {
        if (pair.first != "Menu") {
            if (this->_menuColumn == 0 && i == this->_menuIndex + 2) {
                this->_currentGame = pair.first;
                this->_gameDisplay.tiles.drawText(3, this->_menuIndex + 2, ">", this->_charTable);
            }
            this->_gameDisplay.tiles.drawText(5, i, pair.first, this->_charTable);
            this->_gameDisplay.tiles.drawText(5 + pair.first.size() + 1, i, "(" + std::to_string(this->_highScoresMap[pair.first]) + ")", this->_charTable);
            i++;
        }
    }
    i = 2;
    for (const auto &pair : this->_displays) {
        if (this->_menuColumn == 1 && i == this->_menuIndex + 2) {
            this->_currentDisplay = pair.first;
            this->_gameDisplay.tiles.drawText(22, this->_menuIndex + 2, ">", this->_charTable);
        }
        this->_gameDisplay.tiles.drawText(24, i, pair.first, this->_charTable);
        i++;
    }
}

void Menu::onEvent(Arcade::Event ev)
{
    if (ev.type == Arcade::Event::Type::KEY_DOWN)
    {
        if (ev.data.keyboard.key >= 'a' && ev.data.keyboard.key <= 'z') {
            if (this->_menuIndex == -1 && this->_name.size() < 9) {
                this->_name += ev.data.keyboard.key;
                this->arcade->setName(this->_name);
            }
        }
        if (ev.data.keyboard.key == 'z' || ev.data.keyboard.key == UP_ARROW)
        {
            if (!(this->_menuIndex == -1 && ev.data.keyboard.key == 'z')) {
                this->_menuIndex -= 1;
                if (this->_menuIndex == -2) {
                    if (this->_menuColumn == 0) {
                        this->_menuIndex = this->_nbGame - 1;
                    } else if (this->_menuColumn == 1) {
                        this->_menuIndex = this->_nbDisplay - 1;
                    }
                }
                this->arcade->getDisplay().playSound("menu_button_sound");
            }
        }
        if (ev.data.keyboard.key == 's' || ev.data.keyboard.key == DOWN_ARROW)
        {
            if (!(this->_menuIndex == -1 && ev.data.keyboard.key == 's')) {
                this->_menuIndex += 1;
                if (this->_menuColumn == 0) {
                    if (this->_menuIndex > this->_nbGame - 1) {
                        this->_menuIndex = -1;
                    }
                } else if (this->_menuColumn == 1) {
                    if (this->_menuIndex > this->_nbDisplay - 1) {
                        this->_menuIndex = -1;
                    }
                }
                this->arcade->getDisplay().playSound("menu_button_sound");
            }
        }
        if (ev.data.keyboard.key == 'd' || ev.data.keyboard.key == RIGHT_ARROW) {
            if (this->_menuIndex != -1) {
                if (this->_menuColumn == 0) {
                    this->_menuColumn = 1;
                    if (this->_menuIndex > this->_nbDisplay - 1) {
                        this->_menuIndex = this->_nbDisplay - 1;
                    }
                } else if (this->_menuColumn == 1) {
                    this->_menuColumn = 0;
                    if (this->_menuIndex > this->_nbGame - 1) {
                        this->_menuIndex = this->_nbGame - 1;
                    }
                }
                this->arcade->getDisplay().playSound("menu_button_sound");
            }
        }
        if (ev.data.keyboard.key == 'q' || ev.data.keyboard.key == LEFT_ARROW) {
            if (this->_menuIndex != -1) {
                if (this->_menuColumn == 0) {
                    this->_menuColumn = 1;
                    if (this->_menuIndex > this->_nbDisplay - 1) {
                        this->_menuIndex = this->_nbDisplay - 1;
                    }
                } else if (this->_menuColumn == 1) {
                    this->_menuColumn = 0;
                    if (this->_menuIndex > this->_nbGame - 1) {
                        this->_menuIndex = this->_nbGame - 1;
                    }
                }
                this->arcade->getDisplay().playSound("menu_button_sound");
            }
        }
        if (ev.data.keyboard.key == '\r') {
            if (this->_menuIndex != -1) {
                if (this->_menuColumn == 0) {
                    this->_gameDisplay.music = "";
                    this->arcade->setGame(this->_currentGame);
                } else if (this->_menuColumn == 1) {
                    this->arcade->setDisplay(this->_currentDisplay);
                }
            }
        }
        if (ev.data.keyboard.key == '\b') {
            if (this->_menuIndex == -1) {
                if (this->_name.size() > 0) {
                    this->_name.pop_back();
                    this->arcade->setName(this->_name);
                }
            }
        }
    }
}

extern "C" 
{
    Arcade::LibraryType getType() {
        return Arcade::LibraryType::GAME;
    }

    std::string getName() {
        return "Menu";
    }

    Arcade::IGameModule *getInstance(Arcade::IArcade* arcade) {
        return new Menu(arcade);
    }

    void deleteInstance(Arcade::IGameModule *instance) {
        delete instance;
    }
}
