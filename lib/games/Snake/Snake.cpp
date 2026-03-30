/*
** EPITECH PROJECT, 2024
** Visual Studio Live Share (Espace de travail)
** File description:
** Snake.hpp
*/

#include "Snake.hpp"
#include <fstream>

#define TILE_SIZE 32
#define TOP_SIZE 5
#define DIFFICULTY 2

Snake::Snake(Arcade::IArcade *arcade)
{
    srand(time(NULL));
    this->name = "Snake";
    this->arcade = arcade;
    this->_snakeVelocity = {0, -1};
    this->_mapSize = {30, 15 + TOP_SIZE};

    // Load snake tileset
    this->_assets.tileSets.push_back({"assets/snake/snake_tileset_v3.png", {TILE_SIZE, TILE_SIZE}, {"head", "body", "corner", "background", "apple"}});
    
    // Load font tileset
    std::vector<std::string> charNamesTemp;
    for (int i = 32; i != 127; i++) {
        charNamesTemp.push_back(std::string(1, (char)i));
        this->_charTable[(char)i] = std::string(1, (char)i);
    }
    this->_assets.tileSets.push_back({"assets/font/font_white.png", {TILE_SIZE, TILE_SIZE}, charNamesTemp});
    this->_assets.musics["snake_music"] = "assets/snake/music.ogg";
    this->_assets.sounds["snake_eat"] = "assets/snake/oooh.ogg";

    int readScores = false;
    std::ifstream file("highScores.yaml");
    if (file.is_open()) {
        std::string line;
        std::string currentGame;
        while (std::getline(file, line)) {
            if (line[0] == '#' && line == "#Snake") {
                readScores = true;
                continue;
            }
            if (line.empty() || !readScores) {
                continue;
            }
            if (line[0] == '#') {
                break;
            }
            this->_highScores.push_back(std::stoi(line.substr(line.find(":") + 1, line.size() - (line.find(":") + 1))));
            this->_highScoresNames.push_back(line.substr(0, line.find(":")));
        }
        file.close();
    } else {
        std::cerr << "Unable to open file highScores.yaml" << std::endl;
    }
}

Snake::~Snake()
{
    return;
}

void Snake::initGame()
{
    // Reset game variables
    this->_snakePos.clear();
    this->_snakeDir.clear();
    this->_snakeVelocityList.clear();
    this->_hasMoved = false;
    this->_isAlive = true;
    this->_snakeVelocity = {0, -1};
    this->_timer = 0.0;
    this->_score = 0;
    this->_updateTimer = 0.0;

    // Init snake position
    this->_snakePos.push_back({15, 12});
    this->_snakePos.push_back({15, 13});
    this->_snakePos.push_back({15, 14});
    this->_snakePos.push_back({15, 15});

    // Init snake direction
    this->_snakeDir.push_back(0);
    this->_snakeDir.push_back(0);
    this->_snakeDir.push_back(0);
    this->_snakeDir.push_back(0);
   
    // Init apple position
    this->_applePos = {(float) round(rand() % this->_mapSize.x), (float) round(rand() % (this->_mapSize.y - TOP_SIZE) + TOP_SIZE)};
    
    // Init map
    this->_gameDisplay.tiles = Arcade::TileMap(this->_mapSize);
    for (int y = TOP_SIZE; y != this->_mapSize.y; y++) {
        for (int x = 0; x != this->_mapSize.x; x++) {
            this->_gameDisplay.tiles.setTile(x, y, "background");
        }
    }
    this->_gameDisplay.music = "snake_music";
}

void Snake::updateHighScores()
{
    bool newHighScore = false;
    for (std::size_t i = 0; i != this->_highScores.size(); i++) {
        if (this->_score > this->_highScores[i]) {
            this->_highScores.insert(this->_highScores.begin() + i, this->_score);
            this->_highScoresNames.insert(this->_highScoresNames.begin() + i, arcade->getName() == "" ? "?" : arcade->getName());
            if (this->_highScores.size() > 5) {
                this->_highScores.pop_back();
                this->_highScoresNames.pop_back();
            }
            newHighScore = true;
            break;
        }
    }
    if (!newHighScore && this->_highScores.size() < 5) {
        this->_highScores.push_back(this->_score);
        this->_highScoresNames.push_back(arcade->getName() == "" ? "?" : arcade->getName());
        newHighScore = true;
    }
    if (newHighScore) {
        std::ifstream file("highScores.yaml");
        std::vector<std::string> lines;
        std::string line;
        bool snakeSection = false;
        while (std::getline(file, line)) {
            if (line == "#Snake") {
                snakeSection = true;
            }
            else if (line[0] == '#') {
                snakeSection = false;
                lines.push_back(line);
            }
            else if (!snakeSection) {
                lines.push_back(line);
            }
        }
        file.close();
        std::ofstream outFile("highScores.yaml");
        if (outFile.is_open()) {
            for (const std::string &l : lines) {
                outFile << l << std::endl;
            }
            outFile << "#Snake" << std::endl;
            for (size_t i = 0; i < _highScores.size(); ++i) {
                outFile << _highScoresNames[i] << ": " << _highScores[i] << std::endl;
            }
            outFile.close();
        } else {
            std::cerr << "Unable to open file highScores.yaml" << std::endl;
        }
    }
}

void Snake::gameUpdate(double delta)
{
    // Inc timer
    this->_updateTimer += delta;
    this->_timer += delta;
    // Update snake position every 100ms
    if (this->_updateTimer < 100.0 - (this->_score * DIFFICULTY)) {
        return;
    }
    this->_updateTimer = 0.0;

    // Loose condition
    if (this->_snakePos.front().y + this->_snakeVelocity.y < TOP_SIZE || this->_snakePos.front().y + this->_snakeVelocity.y >= this->_mapSize.y) {
        this->_isAlive = false;
        this->updateHighScores();
    }
    if (this->_snakePos.front().x + this->_snakeVelocity.x < 0 || this->_snakePos.front().x + this->_snakeVelocity.x >= this->_mapSize.x) {
        this->_isAlive = false;
        this->updateHighScores();
    }
    for (std::size_t i = 1; i != this->_snakePos.size(); i++) {
        if (this->_snakePos.front().x == this->_snakePos[i].x && this->_snakePos.front().y == this->_snakePos[i].y) {
            this->_isAlive = false;
            this->updateHighScores();
        }
    }

    // Clear sprites
    this->_gameDisplay.sprites.clear();

    // Save tail position and direction
    Arcade::Vector2f oldPos = this->_snakePos.back();
    double oldDir = this->_snakeDir.back();

    // Update snake position and direction
    for (int i = this->_snakePos.size() - 1; i >= 0; i--) {
        if (i == 0) {
            this->_snakePos[i].x += this->_snakeVelocity.x;
            this->_snakePos[i].y += this->_snakeVelocity.y;
            this->_snakeDir[i] = this->_snakeVelocity.x == 1 ? 90 : this->_snakeVelocity.x == -1 ? 270 : this->_snakeVelocity.y == 1 ? 180 : 0;
        } else {
            this->_snakePos[i].x = this->_snakePos[i - 1].x;
            this->_snakePos[i].y = this->_snakePos[i - 1].y;
            this->_snakeDir[i] = this->_snakeDir[i - 1];
        }
    }
    this->_snakeDir[this->_snakePos.size() - 1] = this->_snakeDir[this->_snakePos.size() - 2];
    if (this->_snakeVelocityList.size() != 0) {
        this->_snakeVelocity = this->_snakeVelocityList.front();
        this->_snakeVelocityList.erase(this->_snakeVelocityList.begin());
        this->_hasMoved = false;
    } else {
        this->_hasMoved = true;
    }

    // Check if snake eat apple
    if (this->_snakePos.front().x == this->_applePos.x && this->_snakePos.front().y == this->_applePos.y) {
        this->_snakePos.push_back(oldPos);
        this->_snakeDir.push_back(oldDir);
        this->_score += 1;
        int index = round(rand() % (this->_mapSize.x * (this->_mapSize.y - TOP_SIZE) - this->_snakePos.size()));
        Arcade::Vector2f applePos;
        applePos.x = 0;
        applePos.y = TOP_SIZE;
        while (index > 0) {
            bool found = false;
            for (std::size_t i = 0; i != this->_snakePos.size(); i++) {
                if (this->_snakePos[i].x == applePos.x && this->_snakePos[i].y == applePos.y) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                index--;
            }
            applePos.x += 1;
            if (applePos.x == this->_mapSize.x) {
                applePos.x = 0;
                applePos.y += 1;
            }
        }
        this->_applePos = applePos;
        this->arcade->getDisplay().playSound("snake_eat");
    }

    // Create sprites
    this->_gameDisplay.sprites.push_back({"apple", {this->_applePos.x, this->_applePos.y}, 0, false, false, true});
    for (std::size_t i = 0; i != this->_snakePos.size(); i++) {
        if (i == 0) {
            this->_gameDisplay.sprites.push_back({"head", {this->_snakePos[i].x, this->_snakePos[i].y}, this->_snakeDir[i], false, false, true});
        } else {
            // Check if it's a corner
            if (i + 1 != this->_snakePos.size() && 
            (this->_snakePos[i - 1].x != this->_snakePos[i + 1].x && this->_snakePos[i - 1].y != this->_snakePos[i + 1].y)) {
                double rotate = 0;
                if ((this->_snakePos[i - 1].y < this->_snakePos[i].y && this->_snakePos[i + 1].x > this->_snakePos[i].x) ||
                    (this->_snakePos[i + 1].y < this->_snakePos[i].y && this->_snakePos[i - 1].x > this->_snakePos[i].x)) {
                    rotate = 0;
                } else if ((this->_snakePos[i - 1].y > this->_snakePos[i].y && this->_snakePos[i + 1].x > this->_snakePos[i].x) ||
                    (this->_snakePos[i + 1].y > this->_snakePos[i].y && this->_snakePos[i - 1].x > this->_snakePos[i].x)) {
                    rotate = 90;
                } else if ((this->_snakePos[i - 1].y > this->_snakePos[i].y && this->_snakePos[i + 1].x < this->_snakePos[i].x) ||
                    (this->_snakePos[i + 1].y > this->_snakePos[i].y && this->_snakePos[i - 1].x < this->_snakePos[i].x)) {
                    rotate = 180;
                } else if ((this->_snakePos[i - 1].y < this->_snakePos[i].y && this->_snakePos[i + 1].x < this->_snakePos[i].x) ||
                    (this->_snakePos[i + 1].y < this->_snakePos[i].y && this->_snakePos[i - 1].x < this->_snakePos[i].x)) {
                    rotate = 270;
                }
                this->_gameDisplay.sprites.push_back({"corner", {this->_snakePos[i].x, this->_snakePos[i].y}, rotate, false, false, true});
            } else {
                this->_gameDisplay.sprites.push_back({"body", {this->_snakePos[i].x, this->_snakePos[i].y}, this->_snakeDir[i], false, false, true});
            }
        }
    }
}

void Snake::gameOverUpdate([[maybe_unused]]double delta)
{
    this->getDisplay().sprites.clear();
    
    // Init map
    this->_gameDisplay.tiles = Arcade::TileMap(this->_mapSize);
    for (int y = TOP_SIZE; y != this->_mapSize.y; y++) {
        for (int x = 0; x != this->_mapSize.x; x++) {
            this->_gameDisplay.tiles.setTile(x, y, "background");
        }
    }

    // Draw game over menu
    std::size_t i = 0;
    this->_gameDisplay.tiles.drawText(10, 0 + TOP_SIZE, "GAME OVER", this->_charTable);
    this->_gameDisplay.tiles.drawText(10, 1 + TOP_SIZE, "SCORE: " + std::to_string(this->_score), this->_charTable);
    this->_gameDisplay.tiles.drawText(10, 3 + TOP_SIZE, "HIGHSCORES:", this->_charTable);
    for (; i != this->_highScores.size(); i++) {
        this->_gameDisplay.tiles.drawText(10, 4 + TOP_SIZE + i, this->_highScoresNames[i] + ": " + std::to_string(this->_highScores[i]), this->_charTable);
    }
    this->_gameDisplay.tiles.drawText(11, 5 + i + TOP_SIZE, "RESTART", this->_charTable);
    this->_gameDisplay.tiles.drawText(11, 6 + i + TOP_SIZE, "MENU", this->_charTable);
    this->_gameDisplay.tiles.drawText(10, 5 + i + TOP_SIZE + this->_menuSelection, ">", this->_charTable);
}

void Snake::onUpdate(double delta)
{
    // Draw top menu
    this->_gameDisplay.tiles.drawText(11, 0, "SNAKE", this->_charTable);
    this->_gameDisplay.tiles.drawText(5, 1, "Time: " + std::to_string((int)this->_timer / 1000), this->_charTable);
    this->_gameDisplay.tiles.drawText(5, 2, "Score: " + std::to_string(this->_score), this->_charTable);
    if (this->_isAlive) {
        this->gameUpdate(delta);
    } else {
        this->gameOverUpdate(delta);
    }
}

void Snake::onEvent(Arcade::Event ev)
{
    if (ev.type == Arcade::Event::Type::KEY_DOWN)
    {
        if (ev.data.keyboard.key == 'z' || ev.data.keyboard.key == 'R')
        {
            if (!this->_isAlive) {
                this->_menuSelection = (this->_menuSelection + 1) % 2;
            } else {
                if (this->_snakeVelocity.y == 1) {
                    return;
                }
                if (this->_snakeVelocity.x != 0 || this->_snakeVelocity.y != -1) {
                    if (!this->_hasMoved || this->_snakeVelocityList.size() != 0){
                        this->_snakeVelocityList.push_back({0, -1});
                        this->_hasMoved = false;
                    } else {
                        this->_snakeVelocity.x = 0;
                        this->_snakeVelocity.y = -1;
                        this->_hasMoved = false;
                    }
                }
            }
        }
        if (ev.data.keyboard.key == 's' || ev.data.keyboard.key == 'Q')
        {
            if (!this->_isAlive) {
                this->_menuSelection = (this->_menuSelection + 1) % 2;
            } else {
                if (this->_snakeVelocity.y == -1) {
                    return;
                }
                if (this->_snakeVelocity.x != 0 || this->_snakeVelocity.y != 1) {
                    if (!this->_hasMoved || this->_snakeVelocityList.size() != 0) {
                        this->_snakeVelocityList.push_back({0, 1});
                        this->_hasMoved = false;
                    } else {
                        this->_snakeVelocity.x = 0;
                        this->_snakeVelocity.y = 1;
                        this->_hasMoved = false;
                    }
                }
            }
        }
        if (ev.data.keyboard.key == 'q' || ev.data.keyboard.key == 'P')
        {
            if (this->_snakeVelocity.x == 1) {
                return;
            }
            if (this->_snakeVelocity.x != -1 || this->_snakeVelocity.y != 0) {
                if (!this->_hasMoved || this->_snakeVelocityList.size() != 0) {
                    this->_snakeVelocityList.push_back({-1, 0});
                    this->_hasMoved = false;
                } else {
                    this->_snakeVelocity.x = -1;
                    this->_snakeVelocity.y = 0;
                    this->_hasMoved = false;
                }
            }
        }
        if (ev.data.keyboard.key == 'd' || ev.data.keyboard.key == 'O')
        {
            if (this->_snakeVelocity.x == -1) {
                return;
            }
            if (this->_snakeVelocity.x != 1 || this->_snakeVelocity.y != 0) {
                if (!this->_hasMoved || this->_snakeVelocityList.size() != 0) {
                    this->_snakeVelocityList.push_back({1, 0});
                    this->_hasMoved = false;
                } else {
                    this->_snakeVelocity.x = 1;
                    this->_snakeVelocity.y = 0;
                    this->_hasMoved = false;
                }
            }
        }
        if (ev.data.keyboard.key == '\r') {
            if (!this->_isAlive) {
                if (this->_menuSelection == 0) {
                    this->initGame();
                } else {
                    this->_gameDisplay.music = "";
                    this->arcade->setGame("Menu");
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
        return "Snake";
    }

    Arcade::IGameModule *getInstance(Arcade::IArcade* arcade) {
        return new Snake(arcade);
    }

    void deleteInstance(Arcade::IGameModule *instance) {
        delete instance;
    }
}
