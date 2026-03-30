/*
** EPITECH PROJECT, 2024
** B-OOP-400-MPL-4-1-arcade-marlon.pegahi
** File description:
** Sfml.cpp
*/

#include "Sfml.hpp"
#include "IArcade.hpp"
#include "Vector.hpp"

Sfml::Sfml(Arcade::IArcade *arcade)
{
    this->_arcade = arcade;
    this->_window.create(sf::VideoMode(1920, 1080), "Arcade - SFML");
    this->_window.setFramerateLimit(60);
}

Sfml::~Sfml()
{
    for (auto &music : this->_musics)
    {
        music.second->stop();
    }
    this->_window.close();
}

void Sfml::display(const Arcade::GameDisplay &display)
{
    Arcade::Vector2i mapSize = display.tiles.getMapSize();
    this->_window.clear();
    // Display TileMap
    for (int y = 0; y < mapSize.y; y++)
    {
        for (int x = 0; x < mapSize.x; x++)
        {
            const std::string &tile = display.tiles.getTile(x, y);
            if (tile.empty())
                continue;
            sf::Sprite sprite(*(this->_tiles[tile].texture));
            sprite.setTextureRect({this->_tiles[tile].rect.x, this->_tiles[tile].rect.y, this->_tiles[tile].rect.width, this->_tiles[tile].rect.height});
            sprite.setPosition(x * this->_tiles[tile].rect.width, y * this->_tiles[tile].rect.height);
            this->_window.draw(sprite);
        }
    }
    // Dipslay Sprites
    for (std::size_t i = 0; i != display.sprites.size(); i++)
    {
        if (display.sprites[i].visible)
        {
            const std::string &tile = display.sprites[i].name;
            sf::Sprite sprite(*(this->_tiles[tile].texture));
            sprite.setTextureRect({this->_tiles[tile].rect.x, this->_tiles[tile].rect.y, this->_tiles[tile].rect.width, this->_tiles[tile].rect.height});
            sprite.setPosition(display.sprites[i].pos.x * this->_tiles[tile].rect.width , display.sprites[i].pos.y * this->_tiles[tile].rect.height);
            sprite.setOrigin(this->_tiles[tile].rect.width / 2, this->_tiles[tile].rect.height / 2);
            sprite.setPosition((display.sprites[i].pos.x * this->_tiles[tile].rect.width) + this->_tiles[tile].rect.width / 2, (display.sprites[i].pos.y * this->_tiles[tile].rect.height) + this->_tiles[tile].rect.height / 2);
            sprite.setRotation(display.sprites[i].rotation);
            this->_window.draw(sprite);
        }
    }
    this->_window.display();
    this->playMusic(display.music, true);
}

void Sfml::loadTileSet(const Arcade::TileSet &tileSet)
{
    sf::Texture *texture = new sf::Texture();
    std::cout << tileSet.texturePath << std::endl;
    texture->loadFromFile(tileSet.texturePath);
    sf::Vector2u imgSize = texture->getSize();
    int k = 0;

    std::cout << imgSize.y << " " << imgSize.x << std::endl;
    std::cout << tileSet.tileSize.y << " " << tileSet.tileSize.x << std::endl;
    for (unsigned int i = 0; i + tileSet.tileSize.y <= imgSize.y; i += tileSet.tileSize.y)
    {
        for (unsigned int j = 0; j + tileSet.tileSize.x <= imgSize.x; j += tileSet.tileSize.x)
        {
            this->_tiles[tileSet.tileNames[k++]] = {texture, {(int)j, (int)i, tileSet.tileSize.x, tileSet.tileSize.y}};
            if ((std::size_t) k == tileSet.tileNames.size())
                return;
        }
    }

    std::cout << "LOADED" << std::endl;
}

void Sfml::loadMusic(const std::string &filename, const std::string &musicId)
{
    std::shared_ptr<sf::Music> music =  std::make_shared<sf::Music>();
    if (!music->openFromFile(filename)) {
        std::cerr << "Failed to load music: " << filename << std::endl;
        return;
    }
    if (this->_musics.find(musicId) != this->_musics.end()) {
        return;
    }
    this->_musics[musicId] = music;
}

void Sfml::loadSound(const std::string &filename, const std::string &soundId)
{
    std::shared_ptr<sf::SoundBuffer> soundBuffer = std::make_shared<sf::SoundBuffer>();
    if (!soundBuffer->loadFromFile(filename)) {
        std::cerr << "Failed to load sound: " << filename << std::endl;
        return;
    }
    this->_soundsBuffer[soundId] = soundBuffer;
    std::shared_ptr<sf::Sound> soundInstance = std::make_shared<sf::Sound>();
    soundInstance->setBuffer(*soundBuffer);
    this->_sounds[soundId] = soundInstance;
}

void Sfml::playMusic(const std::string &musicId, bool repeat)
{
    if (musicId.size() == 0) {
        for (auto &music : this->_musics)
        {
            music.second->stop();
        }
        return;
    }
    if (this->_musics.find(musicId) == this->_musics.end()) {
        std::cerr << "Music not found: " << musicId << std::endl;
        return;
    }
    if (this->_musics[musicId]->getStatus() != sf::Music::Playing) {
        this->_musics[musicId]->setLoop(repeat);
        this->_musics[musicId]->play();
    }
}

void Sfml::playSound(const std::string &soundId)
{
    if (this->_sounds.find(soundId) == this->_sounds.end()) {
        std::cerr << "Sound not found: " << soundId << std::endl;
        return;
    }
    this->_sounds[soundId]->play();
}

Arcade::Event &Sfml::pollEvent()
{
    sf::Event event;
    Arcade::Event::Data data;
    std::string keyChar = "abcdefghijklmnopqrstuvwxyz";

    while (this->_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) {
            this->_event.data = {};
            this->_event.type = Arcade::Event::Type::CLOSE;
            return this->_event;
        }
        if (event.type == sf::Event::KeyPressed)
        {
            data.keyboard.key = '\0';
            if (event.key.code == sf::Keyboard::Key::Escape) {
                this->_event.data = data;
                this->_event.type = Arcade::Event::Type::CLOSE;
                return this->_event;
            }
            if (event.key.code >= 0 && event.key.code <= 25)
            {
                data.keyboard.key = keyChar[event.key.code];
            }
            else
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Up:
                    data.keyboard.key = 'R';
                    break;
                case sf::Keyboard::Key::Down:
                    data.keyboard.key = 'Q';
                    break;
                case sf::Keyboard::Key::Left:
                    data.keyboard.key = 'P';
                    break;
                case sf::Keyboard::Key::Right:
                    data.keyboard.key = 'O';
                    break;
                case sf::Keyboard::Key::Space:
                    data.keyboard.key = ' ';
                    break;
                case sf::Keyboard::Key::Enter:
                    data.keyboard.key = '\r';
                    break;
                case sf::Keyboard::Key::Backspace:
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
        if (event.type == sf::Event::KeyReleased)
        {
            data.keyboard.key = '\0';
            if (event.key.code >= 0 && event.key.code <= 25)
            {
                data.keyboard.key = keyChar[event.key.code];
            }
            else
            {
                switch (event.key.code)
                {
                case sf::Keyboard::Key::Up:
                    data.keyboard.key = 'R';
                    break;
                case sf::Keyboard::Key::Down:
                    data.keyboard.key = 'Q';
                    break;
                case sf::Keyboard::Key::Left:
                    data.keyboard.key = 'P';
                    break;
                case sf::Keyboard::Key::Right:
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
        return "Sfml";
    }

    Arcade::IDisplayModule *getInstance(Arcade::IArcade *arcade)
    {
        return new Sfml(arcade);
    }

    void deleteInstance(Arcade::IDisplayModule *instance)
    {
        delete instance;
    }
}
