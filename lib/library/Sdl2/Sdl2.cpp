/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Sdl2.cpp
*/

#include "Sdl2.hpp"
#include "IArcade.hpp"
#include "Vector.hpp"

Sdl2::Sdl2(Arcade::IArcade *arcade)
{
    this->_arcade = arcade;
    SDL_Init(SDL_INIT_VIDEO);
    this->_window = SDL_CreateWindow("Arcade - SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_SHOWN);
    this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
    }
}

Sdl2::~Sdl2()
{
    for (auto &music : this->_musics)
    {
        Mix_FreeMusic(music.second);
    }
    for (auto &sound : this->_sounds)
    {
        Mix_FreeChunk(sound.second);
    }
    SDL_DestroyRenderer(this->_renderer);
    SDL_DestroyWindow(this->_window);
    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}

void Sdl2::display(const Arcade::GameDisplay &display)
{
    Arcade::Vector2i mapSize = display.tiles.getMapSize();

    SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->_renderer);
    // Display TileMap
    for (int y = 0; y < mapSize.y; y++)
    {
        for (int x = 0; x < mapSize.x; x++)
        {
            const std::string &tile = display.tiles.getTile(x, y);
            if (tile.empty())
                continue;
            SDL_Rect srcRect = {this->_tiles[tile].rect.x, this->_tiles[tile].rect.y, this->_tiles[tile].rect.width, this->_tiles[tile].rect.height};
            SDL_Rect dstRect = {static_cast<int>(x * this->_tiles[tile].rect.width), static_cast<int>(y * this->_tiles[tile].rect.height), this->_tiles[tile].rect.width, this->_tiles[tile].rect.height};
            SDL_RenderCopy(this->_renderer, this->_tiles[tile].texture, &srcRect, &dstRect);
        }
    }
    // Display Sprites
    for (std::size_t i = 0; i != display.sprites.size(); i++)
    {
        if (display.sprites[i].visible)
        {
            const std::string &tile = display.sprites[i].name;
            SDL_Rect srcRect = {this->_tiles[tile].rect.x, this->_tiles[tile].rect.y, this->_tiles[tile].rect.width, this->_tiles[tile].rect.height};
            SDL_Rect dstRect = {static_cast<int>(display.sprites[i].pos.x * this->_tiles[tile].rect.width), static_cast<int>(display.sprites[i].pos.y * this->_tiles[tile].rect.height), this->_tiles[tile].rect.width, this->_tiles[tile].rect.height};
            SDL_RenderCopyEx(this->_renderer, this->_tiles[tile].texture, &srcRect, &dstRect, display.sprites[i].rotation, NULL, SDL_FLIP_NONE);
        }
    }
    SDL_RenderPresent(this->_renderer);
    this->playMusic(display.music, true);
}

void Sdl2::loadTileSet(const Arcade::TileSet &tileSet)
{
    SDL_Texture *texture = IMG_LoadTexture(this->_renderer, tileSet.texturePath.c_str());
    int w = 0;
    int h = 0;
    int k = 0;

    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    std::cout << tileSet.texturePath << std::endl;
    std::cout << h << " " << w << std::endl;
    std::cout << tileSet.tileSize.y << " " << tileSet.tileSize.x << std::endl;
    for (int i = 0; i + tileSet.tileSize.y <= h; i += tileSet.tileSize.y)
    {
        for (int j = 0; j + tileSet.tileSize.x <= w; j += tileSet.tileSize.x)
        {
            this->_tiles[tileSet.tileNames[k++]] = {texture, {(int)j, (int)i, tileSet.tileSize.x, tileSet.tileSize.y}};
            if ((std::size_t)k == tileSet.tileNames.size())
                return;
        }
    }
    std::cout << "LOADED" << std::endl;
}

void Sdl2::loadMusic(const std::string &filename, const std::string &musicId)
{
    Mix_Music *music = Mix_LoadMUS(filename.c_str());

    if (!music) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        return;
    }
    if (this->_musics.find(musicId) != this->_musics.end()) {
        Mix_FreeMusic(music);
        return;
    }
    this->_musics[musicId] = music;
}

void Sdl2::loadSound(const std::string &filename, const std::string &soundId)
{
    Mix_Chunk *sound = Mix_LoadWAV(filename.c_str());

    if (!sound) {
        std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
        return;
    }
    if (this->_sounds.find(soundId) != this->_sounds.end()) {
        Mix_FreeChunk(sound);
        return;
    }
    this->_sounds[soundId] = sound;
}

void Sdl2::playMusic(const std::string &musicId, bool repeat)
{
    if (musicId.size() == 0) {
        if (Mix_PlayingMusic()) {
            Mix_HaltMusic();
        }
        return;
    }
    if (this->_musics.find(musicId) == this->_musics.end()) {
        std::cerr << "Music not found: " << musicId << std::endl;
        return;
    }
    if (!Mix_PlayingMusic()) {
        int temp = Mix_PlayMusic(this->_musics[musicId], repeat ? -1 : 0);
        if (temp == -1) {
            std::cerr << "Failed to play music: " << Mix_GetError() << std::endl;
        }
    }
}

void Sdl2::playSound(const std::string &soundId)
{
    if (this->_sounds.find(soundId) == this->_sounds.end()) {
        std::cerr << "Sound not found: " << soundId << std::endl;
        return;
    }
    Mix_PlayChannel(-1, this->_sounds[soundId], 0);
}

Arcade::Event &Sdl2::pollEvent()
{
    SDL_Event event;
    Arcade::Event::Data data;
    std::string keyChar = "abcdefghijklmnopqrstuvwxyz";

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            this->_event.type = Arcade::Event::Type::CLOSE;
            this->_event.data = {};
            return this->_event;
        }
        if (event.type == SDL_KEYDOWN)
        {
            data.keyboard.key = '\0';
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                this->_event.type = Arcade::Event::Type::CLOSE;
                this->_event.data = {};
                return this->_event;
            }
            if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z)
            {
                data.keyboard.key = keyChar[event.key.keysym.sym - SDLK_a];
            }
            else
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    data.keyboard.key = 'R';
                    break;
                case SDLK_DOWN:
                    data.keyboard.key = 'Q';
                    break;
                case SDLK_LEFT:
                    data.keyboard.key = 'P';
                    break;
                case SDLK_RIGHT:
                    data.keyboard.key = 'O';
                    break;
                case SDLK_SPACE:
                    data.keyboard.key = ' ';
                    break;
                case SDLK_RETURN:
                    data.keyboard.key = '\r';
                    break;
                case SDLK_BACKSPACE:
                    data.keyboard.key = '\b';
                    break;
                default:
                    break;
                }
            }
            this->_event.type = Arcade::Event::Type::KEY_DOWN;
            this->_event.data = data;
            return this->_event;
        }
        if (event.type == SDL_KEYUP)
        {
            data.keyboard.key = '\0';
            if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z)
            {
                data.keyboard.key = keyChar[event.key.keysym.sym - SDLK_a];
            }
            else
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    data.keyboard.key = 'R';
                    break;
                case SDLK_DOWN:
                    data.keyboard.key = 'Q';
                    break;
                case SDLK_LEFT:
                    data.keyboard.key = 'P';
                    break;
                case SDLK_RIGHT:
                    data.keyboard.key = 'O';
                    break;
                default:
                    break;
                }
            }
            this->_event.type = Arcade::Event::Type::KEY_UP;
            this->_event.data = data;
            return this->_event;
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
        return "Sdl2";
    }

    Arcade::IDisplayModule *getInstance(Arcade::IArcade *arcade)
    {
        return new Sdl2(arcade);
    }

    void deleteInstance(Arcade::IDisplayModule *instance)
    {
        delete instance;
    }
}
