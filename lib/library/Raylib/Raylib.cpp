/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Sfml.cpp
*/

#include "Raylib.hpp"
#include "IArcade.hpp"
#include "Vector.hpp"

Raylib::Raylib(Arcade::IArcade *arcade)
{
    this->_arcade = arcade;
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    InitAudioDevice();
    InitWindow(1920, 1080, "Arcade - Raylib");
    SetTargetFPS(60);
}

Raylib::~Raylib()
{
    for (auto &music : this->_musics)
    {
        StopMusicStream(*music.second);
        UnloadMusicStream(*music.second);
    }
    for (auto &sound : this->_sounds)
    {
        UnloadSound(*sound.second);
    }
    CloseAudioDevice();
    CloseWindow();
}

void Raylib::display(const Arcade::GameDisplay &display)
{
    Arcade::Vector2i mapSize = display.tiles.getMapSize();
    //clear window
    BeginDrawing();
    ClearBackground(BLACK);
    // Display TileMap
    for (int y = 0; y < mapSize.y; y++)
    {
        for (int x = 0; x < mapSize.x; x++)
        {
            const std::string &tile = display.tiles.getTile(x, y);
            if (tile.empty())
                continue;
            DrawTextureRec(*(this->_tiles[tile].texture),
            {(float)this->_tiles[tile].rect.x, (float)this->_tiles[tile].rect.y, (float)this->_tiles[tile].rect.width, (float)this->_tiles[tile].rect.height},
            {(float)x * this->_tiles[tile].rect.width, (float)y * this->_tiles[tile].rect.height}, WHITE);
        }
    }

    // Dipslay Sprites
    for (std::size_t i = 0; i != display.sprites.size(); i++)
    {
        if (display.sprites[i].visible)
        {
            const std::string &tile = display.sprites[i].name;
            Rectangle sourceRec = {(float)this->_tiles[tile].rect.x, (float)this->_tiles[tile].rect.y, (float)this->_tiles[tile].rect.width, (float)this->_tiles[tile].rect.height};
            Vector2 origin = {sourceRec.width / 2, sourceRec.height / 2};
            Rectangle destRec = {display.sprites[i].pos.x * this->_tiles[tile].rect.width + origin.x, display.sprites[i].pos.y * this->_tiles[tile].rect.height + origin.y, sourceRec.width, sourceRec.height};
            float rotation = display.sprites[i].rotation;

            DrawTexturePro(*(this->_tiles[tile].texture), sourceRec, destRec, origin, rotation, WHITE);
        }
    }
    EndDrawing();
    this->playMusic(display.music, true);
}

void Raylib::loadTileSet(const Arcade::TileSet &tileSet)
{
    Texture2D *texture = new Texture2D();
    std::cout << tileSet.texturePath << std::endl;
    *texture = LoadTexture(tileSet.texturePath.c_str());
    Arcade::Vector2i imgSize = {texture->width, texture->height};
    int k = 0;

    std::cout << imgSize.y << " " << imgSize.x << std::endl;
    std::cout << tileSet.tileSize.y << " " << tileSet.tileSize.x << std::endl;
    for (int i = 0; i + tileSet.tileSize.y <= imgSize.y; i += tileSet.tileSize.y)
    {
        for (int j = 0; j + tileSet.tileSize.x <= imgSize.x; j += tileSet.tileSize.x)
        {
            this->_tiles[tileSet.tileNames[k++]] = {texture, {(int)j, (int)i, tileSet.tileSize.x, tileSet.tileSize.y}};
            if ((std::size_t) k == tileSet.tileNames.size())
                return;
        }
    }

    std::cout << "LOADED" << std::endl;
}

void Raylib::loadMusic(const std::string &filename, const std::string &musicId)
{
    std::cout << "Loading music: " << filename << std::endl;
    Music *music = new Music(LoadMusicStream(filename.c_str()));
    
    if (this->_musics.find(musicId) != this->_musics.end()) {
        return;
    }
    std::cout << "Music loaded: " << filename << " with id: " << musicId << "\n";
    this->_musics[musicId] = music;
}

void Raylib::loadSound(const std::string &filename, const std::string &soundId)
{
    Sound *sound = new Sound(LoadSound(filename.c_str()));
    if (this->_sounds.find(soundId) != this->_sounds.end()) {
        return;
    }
    std::cout << "Sound loaded: " << filename << " with id: " << soundId << "\n";
    this->_sounds[soundId] = sound;
}

void Raylib::playMusic(const std::string &musicId, [[maybe_unused]]bool repeat)
{
    if (musicId.size() == 0) {
        for (auto &music : this->_musics)
        {
            std::cout << "Stoping music: " << music.first << std::endl;
            StopMusicStream(*music.second);
        }
        return;
    }
    if (this->_musics.find(musicId) == this->_musics.end())
    {
        std::cerr << "Music not found: " << musicId << std::endl;
        return;
    }
    if (!IsMusicStreamPlaying(*this->_musics[musicId])) {
        std::cout << "Playing music: " << musicId << std::endl;
        PlayMusicStream(*this->_musics[musicId]);
    }
    UpdateMusicStream(*this->_musics[musicId]);
}

void Raylib::playSound(const std::string &soundId)
{
    std::cout << "Playing sound: " << soundId << std::endl;
    if (this->_sounds.find(soundId) == this->_sounds.end())
    {
        std::cerr << "Sound not found: " << soundId << std::endl;
        return;
    }
    PlaySound(*this->_sounds[soundId]);
    std::cout << "Sound played: " << soundId << std::endl;
}

Arcade::Event &Raylib::pollEvent()
{
    Arcade::Event::Data data;
    std::string keyChar = "abcdefghijklmnopqrstuvwxyz";
    Arcade::Event::Type eventType = Arcade::Event::Type::NONE;

    data.keyboard.key = 0;
    if (IsKeyPressed(KEY_ESCAPE)) {
        this->_event.type = Arcade::Event::Type::CLOSE;
        this->_event.data = {};
        return this->_event;
    }

    if (this->_lastKeyPressed == -2) {
        this->_lastKeyPressed = -1;
        this->_event.type = Arcade::Event::Type::NONE;
        this->_event.data = {};
        return this->_event;
    }
    for (int i = 0; i < 26; i++)
    {
        if (IsKeyPressed(i + KEY_A))
        {
            if (this->_lastKeyPressed == i + KEY_A) {
                data.keyboard.key = keyChar[i];
                eventType = Arcade::Event::Type::NONE;
                break;
            }
            this->_lastKeyPressed = i + KEY_A;
            data.keyboard.key = keyChar[i];
            eventType = Arcade::Event::Type::KEY_DOWN;
        }
        if (IsKeyReleased(i + KEY_A))
        {
            this->_lastKeyPressed = -2;
            data.keyboard.key = keyChar[i];
            eventType = Arcade::Event::Type::KEY_UP;
        }
    }

    if (IsKeyPressed(KEY_UP))
    {
        data.keyboard.key = 'R';
        if (this->_lastKeyPressed == data.keyboard.key) {
            eventType = Arcade::Event::Type::NONE;
            this->_event.type = eventType;
            this->_event.data = data;
            return this->_event;
        }
        this->_lastKeyPressed = 'R';
        eventType = Arcade::Event::Type::KEY_DOWN;
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        data.keyboard.key = 'Q';
        if (this->_lastKeyPressed == data.keyboard.key) {
            eventType = Arcade::Event::Type::NONE;
            this->_event.type = eventType;
            this->_event.data = data;
            return this->_event;
        }
        this->_lastKeyPressed = 'Q';
        eventType = Arcade::Event::Type::KEY_DOWN;
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        data.keyboard.key = 'P';
        if (this->_lastKeyPressed == data.keyboard.key) {
            eventType = Arcade::Event::Type::NONE;
            this->_event.type = eventType;
            this->_event.data = data;
            return this->_event;
        }
        this->_lastKeyPressed = 'P';
        eventType = Arcade::Event::Type::KEY_DOWN;
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
        data.keyboard.key = 'O';
        if (this->_lastKeyPressed == data.keyboard.key) {
            eventType = Arcade::Event::Type::NONE;
            this->_event.type = eventType;
            this->_event.data = data;
            return this->_event;
        }
        this->_lastKeyPressed = 'O';
        eventType = Arcade::Event::Type::KEY_DOWN;
    }
    if (IsKeyPressed(KEY_SPACE))
    {
        data.keyboard.key = ' ';
        if (this->_lastKeyPressed == data.keyboard.key) {
            eventType = Arcade::Event::Type::NONE;
            this->_event.type = eventType;
            this->_event.data = data;
            return this->_event;
        }
        this->_lastKeyPressed = ' ';
        eventType = Arcade::Event::Type::KEY_DOWN;
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        data.keyboard.key = '\r';
        if (this->_lastKeyPressed == data.keyboard.key) {
            eventType = Arcade::Event::Type::NONE;
            this->_event.type = eventType;
            this->_event.data = data;
            return this->_event;
        }
        this->_lastKeyPressed = '\r';
        eventType = Arcade::Event::Type::KEY_DOWN;
    }
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        data.keyboard.key = '\b';
        if (this->_lastKeyPressed == data.keyboard.key) {
            eventType = Arcade::Event::Type::NONE;
            this->_event.type = eventType;
            this->_event.data = data;
            return this->_event;
        }
        this->_lastKeyPressed = '\b';
        eventType = Arcade::Event::Type::KEY_DOWN;
    }

    if (IsKeyReleased(KEY_UP))
    {
        data.keyboard.key = 'R';
        this->_lastKeyPressed = -2;
        eventType = Arcade::Event::Type::KEY_UP;
    }
    if (IsKeyReleased(KEY_DOWN))
    {
        data.keyboard.key = 'Q';
        this->_lastKeyPressed = -2;
        eventType = Arcade::Event::Type::KEY_UP;
    }
    if (IsKeyReleased(KEY_LEFT))
    {
        data.keyboard.key = 'P';
        this->_lastKeyPressed = -2;
        eventType = Arcade::Event::Type::KEY_UP;
    }
    if (IsKeyReleased(KEY_RIGHT))
    {
        data.keyboard.key = 'O';
        this->_lastKeyPressed = -2;
        eventType = Arcade::Event::Type::KEY_UP;
    }
    if (IsKeyReleased(KEY_SPACE))
    {
        data.keyboard.key = ' ';
        this->_lastKeyPressed = -2;
        eventType = Arcade::Event::Type::KEY_UP;
    }
    if (IsKeyReleased(KEY_ENTER))
    {
        data.keyboard.key = '\r';
        this->_lastKeyPressed = -2;
        eventType = Arcade::Event::Type::KEY_UP;
    }
    if (IsKeyReleased(KEY_BACKSPACE))
    {
        data.keyboard.key = '\b';
        this->_lastKeyPressed = -2;
        eventType = Arcade::Event::Type::KEY_UP;
    }
    this->_event.type = eventType;
    this->_event.data = data;
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
        return "Raylib";
    }

    Arcade::IDisplayModule *getInstance(Arcade::IArcade *arcade)
    {
        return new Raylib(arcade);
    }

    void deleteInstance(Arcade::IDisplayModule *instance)
    {
        delete instance;
    }
}
