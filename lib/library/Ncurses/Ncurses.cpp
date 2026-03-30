/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Ncurses.cpp
*/

#include "Ncurses.hpp"
#include "IArcade.hpp"
#include "Vector.hpp"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

#define NC_KEY_DOWN 0402

#undef KEY_DOWN

Ncurses::Ncurses(Arcade::IArcade *arcade)
{
    this->_arcade = arcade;
    this->_window = initscr();
    cbreak();
    keypad(this->_window, TRUE);
    noecho();
    curs_set(0);
    start_color();
    nodelay(stdscr, TRUE);

    init_pair(NCURSES_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(NCURSES_RED, COLOR_RED, COLOR_RED);
    init_pair(NCURSES_GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(NCURSES_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(NCURSES_BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(NCURSES_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(NCURSES_CYAN, COLOR_CYAN, COLOR_CYAN);
    init_pair(NCURSES_WHITE, COLOR_WHITE, COLOR_WHITE);

    this->_colorList["black"] = NCURSES_BLACK;
    this->_colorList["red"] = NCURSES_RED;
    this->_colorList["green"] = NCURSES_GREEN;
    this->_colorList["yellow"] = NCURSES_YELLOW;
    this->_colorList["blue"] = NCURSES_BLUE;
    this->_colorList["magenta"] = NCURSES_MAGENTA;
    this->_colorList["cyan"] = NCURSES_CYAN;
    this->_colorList["white"] = NCURSES_WHITE;
}

void Ncurses::display(const Arcade::GameDisplay &display)
{
    auto frameDuration = std::chrono::milliseconds(1000) / 60;
    auto start = std::chrono::high_resolution_clock::now();
    Arcade::Vector2i mapSize = display.tiles.getMapSize();

    clear();
    for (int y = 0; y < mapSize.y; y++)
    {
        for (int x = 0; x < mapSize.x; x++)
        {
            const std::string &tile = display.tiles.getTile(x, y);
            if (tile.empty())
                continue;
            attron(COLOR_PAIR(this->_tiles[tile].texture->color));
            mvprintw(y, x, "%s", this->_tiles[tile].texture->textToDisplay.c_str());
            attroff(COLOR_PAIR(this->_tiles[tile].texture->color));
        }
    }
    for (std::size_t i = 0; i != display.sprites.size(); i++)
    {
        if (display.sprites[i].visible)
        {
            const std::string &tile = display.sprites[i].name;
            attron(COLOR_PAIR(this->_tiles[tile].texture->color));
            mvprintw(display.sprites[i].pos.y, display.sprites[i].pos.x, "%s", this->_tiles[tile].texture->textToDisplay.c_str());
            attroff(COLOR_PAIR(this->_tiles[tile].texture->color));
        }
    }
    refresh();
    auto frameEnd = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - start);
    auto timeToWait = frameDuration - elapsedTime;
    if (timeToWait.count() > 0)
    {
        std::this_thread::sleep_for(timeToWait);
    }
}

void Ncurses::loadTileSet(const Arcade::TileSet &tileSet)
{
    size_t dotPos = tileSet.texturePath.rfind('.');
    std::string ncTexturePath = tileSet.texturePath.substr(0, dotPos) + ".txt";
    int k = 0;

    std::ifstream file(ncTexturePath);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << ncTexturePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::size_t pos = line.find(';');
        std::string color = line.substr(0, pos);
        std::string character = line.substr(pos + 1);
        std::string charracterBis = character;
        this->_textures.push_back(std::make_shared<NcursesTexture>(this->_colorList[color], charracterBis));
    }

    file.close();

    std::cout << tileSet.tileSize.y << " " << tileSet.tileSize.x << std::endl;
    for (int i = 0; i + 1 <= tileSet.tileSize.y; i++)
    {
        for (int j = 0; j + 1 <= tileSet.tileSize.x; j++)
        {
            this->_tiles[tileSet.tileNames[k]] = {(this->_textures[_textureIndex++].get()), {(int)j, (int)i, tileSet.tileSize.x, tileSet.tileSize.y}};
            k++;
            if ((std::size_t)k == tileSet.tileNames.size())
            {
                return;
            }
        }
    }
}

void Ncurses::loadMusic([[maybe_unused]] const std::string &filename, [[maybe_unused]] const std::string &musicId)
{
    // ncurses does not support music
}

void Ncurses::loadSound([[maybe_unused]] const std::string &filename, [[maybe_unused]] const std::string &soundId)
{
    // ncurses does not support sound
}

void Ncurses::playMusic([[maybe_unused]] const std::string &soundId, [[maybe_unused]] bool repeat)
{
    // ncurses does not support music
}

void Ncurses::playSound([[maybe_unused]] const std::string &id)
{
    // ncurses does not support sound
}

Arcade::Event &Ncurses::pollEvent()
{
    int ch = getch();
    Arcade::Event::Data data;
    std::string keyChar = "abcdefghijklmnopqrstuvwxyz";

    if (ch == 27) {
        this->_event.type = Arcade::Event::Type::CLOSE;
        this->_event.data = {};
        return this->_event;
    }
    data.keyboard.key = '\0';
    if (ch >= 'a' && ch <= 'z')
    {
        data.keyboard.key = ch;
        this->_event.type = Arcade::Event::Type::KEY_DOWN;
        this->_event.data = data;
        return this->_event;
    }
    else
    {
        switch (ch)
        {
        case KEY_UP:
            data.keyboard.key = 'R';
            this->_event.type = Arcade::Event::Type::KEY_DOWN;
            this->_event.data = data;
            return this->_event;
        case NC_KEY_DOWN:
            data.keyboard.key = 'Q';
            this->_event.type = Arcade::Event::Type::KEY_DOWN;
            this->_event.data = data;
            return this->_event;
        case KEY_LEFT:
            data.keyboard.key = 'P';
            this->_event.type = Arcade::Event::Type::KEY_DOWN;
            this->_event.data = data;
            return this->_event;
        case KEY_RIGHT:
            data.keyboard.key = 'O';
            this->_event.type = Arcade::Event::Type::KEY_DOWN;
            this->_event.data = data;
            return this->_event;
        case ' ':
            data.keyboard.key = ' ';
            this->_event.type = Arcade::Event::Type::KEY_DOWN;
            this->_event.data = data;
            return this->_event;
        case '\n':
            data.keyboard.key = '\r';
            this->_event.type = Arcade::Event::Type::KEY_DOWN;
            this->_event.data = data;
            return this->_event;
        case KEY_BACKSPACE:
            data.keyboard.key = '\b';
            this->_event.type = Arcade::Event::Type::KEY_DOWN;
            this->_event.data = data;
            return this->_event;
        default:
            break;
        }
    }
    this->_event.type = Arcade::Event::Type::NONE;
    this->_event.data = {};
    return this->_event;
}

extern "C"
{
    Arcade::LibraryType getType()
    {
        return Arcade::LibraryType::DISPLAY;
    }

    std::string getName()
    {
        return "Ncurses";
    }

    Arcade::IDisplayModule *getInstance(Arcade::IArcade *arcade)
    {
        return new Ncurses(arcade);
    }

    void deleteInstance(Arcade::IDisplayModule *instance)
    {
        delete instance;
    }
}
