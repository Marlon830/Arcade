/*
** EPITECH PROJECT, 2024
** Visual Studio Live Share (Espace de travail)
** File description:
** Centipede.cpp
*/

#include "Centipede.hpp"
#include <fstream>

#define TILE_SIZE 32
#define TOP_SIZE 5

Centipede::Centipede(Arcade::IArcade *arcade)
{
    srand(time(NULL));
    this->name = "Centipede";
    this->arcade = arcade;
    this->_mapSize = {20, 20 + TOP_SIZE};

    // Load centipede tileset
    this->_assets.tileSets.push_back({"assets/centipede/centipede_tileset_v4.png", {TILE_SIZE, TILE_SIZE}, {"centipede_head", "centipede_body", "centipede_turn", "obstacle5", "obstacle4", "obstacle3", "obstacle2", "obstacle1", "background", "player", "missile"}});
    this->_assets.musics["centipede_music"] = "assets/centipede/music.ogg";
    this->_assets.sounds["centipede_hit"] = "assets/centipede/centipede_hit.ogg";
    this->_assets.sounds["obstacle_hit"] = "assets/centipede/obstacle_hit.ogg";
    
    // Load font tileset
    std::vector<std::string> charNamesTemp;
    for (int i = 32; i != 127; i++) {
        charNamesTemp.push_back(std::string(1, (char)i));
        this->_charTable[(char)i] = std::string(1, (char)i);
    }
    this->_assets.tileSets.push_back({"assets/font/font_white.png", {TILE_SIZE, TILE_SIZE}, charNamesTemp});

    int readScores = false;
    std::ifstream file("highScores.yaml");
    if (file.is_open()) {
        std::string line;
        std::string currentGame;
        while (std::getline(file, line)) {
            if (line[0] == '#' && line == "#Centipede") {
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

void Centipede::initGame()
{
    this->_playerAlive = true;
    this->_timer = 0.0;
    this->_score = 0;
    this->_updateTimer = 0.0;
    this->_nbCentipede = 0;
    this->_centipedCanMove = 0;
    this->_centipede.clear();
    this->_obstacles.clear();

    this->_centipede.push_back({{1, 0}, {8, 5}, 90, false, false, false});
    this->_centipede.push_back({{1, 0}, {7, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {6, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {5, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {4, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {3, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {2, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {1, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {0, 5}, 90, true, false, false});

    for (int y = 0; y != (int)(this->_mapSize.y - TOP_SIZE - 1); y++) {
        std::vector<int> row;
        for (int x = 0; x != this->_mapSize.x; x++) {
            row.push_back(0);
        }
        this->_obstacles.push_back(row);
    }
    // Add 10 random obstacles
    for (int i = 0; i != 10; i++) {
        int x = rand() % this->_mapSize.x;
        int y = rand() % (this->_mapSize.y - TOP_SIZE - 1);
        this->_obstacles[y][x] = 5;
    }

    this->_playerPos = {(float)(this->_mapSize.x - 1) / 2, this->_mapSize.y * (float)0.9};
    this->_playerVelocity = {0, 0};
    
    this->_missileExists = false;
    this->_missilePos = {0, 0};
    this->_missileVelocity = {0, -1};

    // Init map
    this->_gameDisplay.tiles = Arcade::TileMap(this->_mapSize);
    for (int y = TOP_SIZE; y != this->_mapSize.y; y++) {
        for (int x = 0; x != this->_mapSize.x; x++) {
            this->_gameDisplay.tiles.setTile(x, y, "background");
        }
    }
    this->_gameDisplay.music = "centipede_music";
}

void Centipede::resetCentipede()
{
    this->_nbCentipede += 1;
    this->_centipede.clear();
    this->_centipede.push_back({{1, 0}, {8, 5}, 90, false, false, false});
    this->_centipede.push_back({{1, 0}, {7, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {6, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {5, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {4, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {3, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {2, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {1, 5}, 90, true, false, false});
    this->_centipede.push_back({{1, 0}, {0, 5}, 90, true, false, false});
    if (this->_nbCentipede == 20) {
        this->_obstacles.clear();
        for (int y = 0; y != (int)(this->_mapSize.y - TOP_SIZE - 1); y++) {
            std::vector<int> row;
            for (int x = 0; x != this->_mapSize.x; x++) {
                row.push_back(0);
            }
            this->_obstacles.push_back(row);
        }
        for (int i = 0; i != 10; i++) {
            int x = rand() % this->_mapSize.x;
            int y = rand() % (this->_mapSize.y - TOP_SIZE - 1);
            this->_obstacles[y][x] = 5;
        }
        this->_nbCentipede = 0;
    }
}

void Centipede::updateHighScores()
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
        bool centipedeSection = false;
        while (std::getline(file, line)) {
            if (line == "#Centipede") {
                centipedeSection = true;
            }
            else if (line[0] == '#') {
                centipedeSection = false;
                lines.push_back(line);
            }
            else if (!centipedeSection) {
                lines.push_back(line);
            }
        }
        file.close();
        std::ofstream outFile("highScores.yaml");
        if (outFile.is_open()) {
            for (const std::string &l : lines) {
                outFile << l << std::endl;
            }
            outFile << "#Centipede" << std::endl;
            for (size_t i = 0; i < _highScores.size(); ++i) {
                outFile << _highScoresNames[i] << ": " << _highScores[i] << std::endl;
            }
            outFile.close();
        } else {
            std::cerr << "Unable to open file highScores.yaml" << std::endl;
        }
    }
}

void Centipede::gameUpdate(double delta)
{
    this->_updateTimer += delta;
    this->_timer += delta;
    // Update position every 16.666ms
    if (this->_updateTimer < 16.666) {
        return;
    }
    this->_centipedCanMove += 0.25;
    this->_updateTimer = 0.0;

    // Clear sprites
    this->_gameDisplay.sprites.clear();

    // Update player position
    if (this->_playerVelocity.x != 0 && this->_playerVelocity.y != 0) {
        this->_playerPos.x += this->_playerVelocity.x * 0.2;
        this->_playerPos.y += this->_playerVelocity.y * 0.2;
    } else {
        this->_playerPos.x += this->_playerVelocity.x * 0.4;
        this->_playerPos.y += this->_playerVelocity.y * 0.4;
    }

    if (this->_playerPos.x < 0) {
        this->_playerPos.x = 0;
    }
    if (this->_playerPos.y < (this->_mapSize.y - TOP_SIZE) * 0.8 + TOP_SIZE) {
        this->_playerPos.y = (this->_mapSize.y - TOP_SIZE) * 0.8 + TOP_SIZE;
    }
    if (this->_playerPos.x > this->_mapSize.x - 1) {
        this->_playerPos.x = this->_mapSize.x - 1;
    }
    if (this->_playerPos.y > this->_mapSize.y - 1) {
        this->_playerPos.y = this->_mapSize.y - 1;
    }

    // Update centipede position
    if (this->_centipedCanMove >= 1) {
        this->_centipedCanMove = 0;
        for (int i = this->_centipede.size() - 1; i >= 0; i--) {
            if (this->_centipede[i].isDead) {
                continue;
            }
            if (this->_centipede[i].isLinked) {
                this->_centipede[i].pos.x = this->_centipede[i - 1].pos.x;
                this->_centipede[i].pos.y = this->_centipede[i - 1].pos.y;
                this->_centipede[i].centipedeVelocity = this->_centipede[i - 1].centipedeVelocity;
                if (this->_centipede[i - 1].isLinked) {
                    this->_centipede[i].rotate = this->_centipede[i - 1].rotate;
                } else {
                    if (this->_centipede[i].centipedeVelocity.x == 1) {
                        this->_centipede[i].rotate = 90;
                    } else if (this->_centipede[i].centipedeVelocity.x == -1) {
                        this->_centipede[i].rotate = 270;
                    } else if (this->_centipede[i].centipedeVelocity.y == 1) {
                        this->_centipede[i].rotate = 0;
                    } else if (this->_centipede[i].centipedeVelocity.y == -1) {
                        this->_centipede[i].rotate = 180;
                    }
                }
            } else {
                this->_centipede[i].pos.x += this->_centipede[i].centipedeVelocity.x;
                this->_centipede[i].pos.y += this->_centipede[i].centipedeVelocity.y;
                if (this->_centipede[i].goDown) {
                    if ((int)(this->_centipede[i].pos.y + 1 - TOP_SIZE) <= (int)(this->_obstacles.size() - 1) && this->_obstacles[(int)this->_centipede[i].pos.y - TOP_SIZE + 1][(int)this->_centipede[i].pos.x - this->_centipede[i].centipedeVelocity.x] != 0) {
                        this->_centipede[i].pos.y += 1;
                        this->_centipede[i].goDown = false;
                        if (this->_centipede[i].pos.y >= this->_mapSize.y) {
                            this->_centipede[i].isDead = true;
                            this->_score -= 10;
                            if (i + 1 < (int)this->_centipede.size()) {
                                this->_centipede[i + 1].isLinked = false;
                            }
                            break;
                        }
                        continue;
                    }
                }
                if (this->_centipede[i].pos.x < 0) {
                    if ((int)(this->_centipede[i].pos.y + 1 - TOP_SIZE) <= (int)(this->_obstacles.size() - 1) && this->_obstacles[(int)this->_centipede[i].pos.y - TOP_SIZE + 1][(int)this->_centipede[i].pos.x + 1] != 0) {
                        this->_centipede[i].pos.x += 1;
                        this->_centipede[i].centipedeVelocity.x = 1;
                        this->_centipede[i].goDown = true;
                    } else {
                        this->_centipede[i].pos.x += 1;
                        this->_centipede[i].pos.y += 1;
                        this->_centipede[i].centipedeVelocity.x = 1;
                    }
                }
                if (this->_centipede[i].pos.x >= this->_mapSize.x) {
                    if ((int)(this->_centipede[i].pos.y + 1 - TOP_SIZE) <= (int)(this->_obstacles.size() - 1) && this->_obstacles[(int)this->_centipede[i].pos.y - TOP_SIZE + 1][(int)this->_centipede[i].pos.x - 1] != 0) {
                        this->_centipede[i].pos.x = this->_mapSize.x - 1;
                        this->_centipede[i].centipedeVelocity.x = -1;
                        this->_centipede[i].goDown = true;
                    } else {
                        this->_centipede[i].pos.x = this->_mapSize.x - 1;
                        this->_centipede[i].pos.y += 1;
                        this->_centipede[i].centipedeVelocity.x = -1;
                    }
                }
                if ((int)(this->_centipede[i].pos.y - TOP_SIZE) <= (int)(this->_obstacles.size() - 1)) {
                    if (this->_obstacles[(int)this->_centipede[i].pos.y - TOP_SIZE][(int)this->_centipede[i].pos.x] != 0) {
                        if ((int)(this->_centipede[i].pos.y + 1 - TOP_SIZE) <= (int)(this->_obstacles.size() - 1) && this->_obstacles[(int)this->_centipede[i].pos.y - TOP_SIZE + 1][(int)this->_centipede[i].pos.x - this->_centipede[i].centipedeVelocity.x] != 0) {
                            this->_centipede[i].pos.x -= this->_centipede[i].centipedeVelocity.x;
                            this->_centipede[i].centipedeVelocity.x = -(this->_centipede[i].centipedeVelocity.x);
                            this->_centipede[i].goDown = true;
                        } else {
                            this->_centipede[i].pos.x -= this->_centipede[i].centipedeVelocity.x;
                            this->_centipede[i].pos.y += 1;
                            this->_centipede[i].centipedeVelocity.x = -(this->_centipede[i].centipedeVelocity.x);
                        }
                    }
                }
                if (this->_centipede[i].pos.y >= this->_mapSize.y) {
                    this->_centipede[i].isDead = true;
                    this->_score -= 10;
                    if (i + 1 < (int)this->_centipede.size()) {
                        this->_centipede[i + 1].isLinked = false;
                    }
                    break;
                }
                if (this->_centipede[i].centipedeVelocity.x == 1) {
                    this->_centipede[i].rotate = 90;
                } else if (this->_centipede[i].centipedeVelocity.x == -1) {
                    this->_centipede[i].rotate = 270;
                } else if (this->_centipede[i].centipedeVelocity.y == 1) {
                    this->_centipede[i].rotate = 0;
                } else if (this->_centipede[i].centipedeVelocity.y == -1) {
                    this->_centipede[i].rotate = 180;
                }
            }
        }
        // Check if the centipede is dead
        for (std::size_t i = 0; i != this->_centipede.size(); i++) {
            if (!this->_centipede[i].isDead) {
                break;
            }
            if (i == this->_centipede.size() - 1 && this->_centipede[i].isDead) {
                this->resetCentipede();
            }
        }
    }

    // Update missile
    if (_missileExists) {
        this->_missilePos.y += this->_missileVelocity.y * 0.5;
        if (this->_missilePos.y < TOP_SIZE) {
            this->_missileExists = false;
        } else {
            // Check if the missile hit an obstacle
            if ((int)this->_missilePos.y - TOP_SIZE <= (int)(this->_obstacles.size() - 1) && this->_obstacles[(int)this->_missilePos.y - TOP_SIZE][(int)(this->_missilePos.x + 0.5)] != 0) {
                this->_missileExists = false;
                this->_obstacles[(int)this->_missilePos.y - TOP_SIZE][(int)(this->_missilePos.x + 0.5)] -= 1;
                this->arcade->getDisplay().playSound("obstacle_hit");
            }
            for (std::size_t i = 0; i != this->_centipede.size(); i++) {
                if (!this->_missileExists) {
                    break;
                }
                // Check if the missile hit a centipede
                if (this->_centipede[i].pos.x == (int)(this->_missilePos.x + 0.5) && this->_centipede[i].pos.y == (int)this->_missilePos.y) {
                    this->_centipede[i].isDead = true;
                    this->_centipede[i].isLinked = false;
                    if (i + 1 != this->_centipede.size()) {
                        this->_centipede[i + 1].isLinked = false;
                    }
                    this->_missileExists = false;
                    this->_score += 100;
                    this->_obstacles[this->_centipede[i].pos.y - TOP_SIZE][this->_centipede[i].pos.x] = 5;
                    this->arcade->getDisplay().playSound("centipede_hit");
                }
            }
        }
    }

    // Loop over the obstacles to check if the player is on one of them
    for (int y = 0; y != (int)(this->_obstacles.size()); y++) {
        for (int x = 0; x != (int)(this->_obstacles[y].size()); x++) {
            if (this->_obstacles[y][x] != 0) {
                if (this->_playerPos.x >= x && this->_playerPos.x < x + 1 && this->_playerPos.y >= y + TOP_SIZE && this->_playerPos.y < y + 1 + TOP_SIZE) {
                    if (std::abs(this->_playerPos.x - (x + 1)) < std::abs(this->_playerPos.y - (y + 1 + TOP_SIZE))) {
                        this->_playerPos.x = (int)(this->_playerPos.x + 1);
                    } else if (std::abs(this->_playerPos.x - (x + 1)) > std::abs(this->_playerPos.y - (y + 1 + TOP_SIZE))) {
                        this->_playerPos.y = (int)(this->_playerPos.y + 1);
                    } else {
                        this->_playerPos.x = (int)(this->_playerPos.x + 1);
                        this->_playerPos.y = (int)(this->_playerPos.y + 1);
                    }
                }
                if (this->_playerPos.x < x && this->_playerPos.x + 1 > x && this->_playerPos.y >= y + TOP_SIZE && this->_playerPos.y < y + 1 + TOP_SIZE) {
                    if (std::abs(this->_playerPos.x + 1 - x) < std::abs(this->_playerPos.y - (y + 1 + TOP_SIZE))) {
                        this->_playerPos.x = (int)this->_playerPos.x;
                    } else if (std::abs(this->_playerPos.x - (x + 1)) > std::abs(this->_playerPos.y - (y + 1 + TOP_SIZE))) {
                        this->_playerPos.y = (int)(this->_playerPos.y + 1);
                    } else {
                        this->_playerPos.x = (int)this->_playerPos.x;
                        this->_playerPos.y = (int)(this->_playerPos.y + 1);
                    }
                }
                if (this->_playerPos.x >= x && this->_playerPos.x < x + 1 && this->_playerPos.y < y + TOP_SIZE && this->_playerPos.y + 1 > y + TOP_SIZE) {
                    if (std::abs(this->_playerPos.x - (x + 1)) < std::abs(this->_playerPos.y + 1 - (y + TOP_SIZE))) {
                        this->_playerPos.x = (int)(this->_playerPos.x + 1);
                    } else if (std::abs(this->_playerPos.x - (x + 1)) > std::abs(this->_playerPos.y + 1 - (y + TOP_SIZE))) {
                        this->_playerPos.y = (int)this->_playerPos.y;
                    } else {
                        this->_playerPos.x = (int)(this->_playerPos.x + 1);
                        this->_playerPos.y = (int)this->_playerPos.y;
                    }
                }
                if (this->_playerPos.x < x && this->_playerPos.x + 1 > x && this->_playerPos.y < y + TOP_SIZE && this->_playerPos.y + 1 > y + TOP_SIZE) {
                    if (std::abs(this->_playerPos.x + 1 - x) < std::abs(this->_playerPos.y + 1 - (y + TOP_SIZE))) {
                        this->_playerPos.x = (int)this->_playerPos.x;
                    } else if (std::abs(this->_playerPos.x + 1 - x) > std::abs(this->_playerPos.y + 1 - (y + TOP_SIZE))) {
                        this->_playerPos.y = (int)this->_playerPos.y;
                    } else {
                        this->_playerPos.x = (int)this->_playerPos.x + 1;
                        this->_playerPos.y = (int)this->_playerPos.y;
                    }
                }
            }
        }
    }

    // Loop over the centipede to check if the player is on one of them
    for (std::size_t i = 0; i != this->_centipede.size(); i++) {
        if (!this->_centipede[i].isDead) {
            if (this->_playerPos.x >= this->_centipede[i].pos.x && this->_playerPos.x < this->_centipede[i].pos.x + 1 && this->_playerPos.y >= this->_centipede[i].pos.y && this->_playerPos.y < this->_centipede[i].pos.y + 1) {
                this->_playerAlive = false;
                this->updateHighScores();
            } else if (this->_playerPos.x < this->_centipede[i].pos.x && this->_playerPos.x + 1 > this->_centipede[i].pos.x && this->_playerPos.y >= this->_centipede[i].pos.y && this->_playerPos.y < this->_centipede[i].pos.y + 1) {
                this->_playerAlive = false;
                this->updateHighScores();
            } else if (this->_playerPos.x >= this->_centipede[i].pos.x && this->_playerPos.x < this->_centipede[i].pos.x + 1 && this->_playerPos.y < this->_centipede[i].pos.y && this->_playerPos.y + 1 > this->_centipede[i].pos.y) {
                this->_playerAlive = false;
                this->updateHighScores();
            } else if (this->_playerPos.x < this->_centipede[i].pos.x && this->_playerPos.x + 1 > this->_centipede[i].pos.x && this->_playerPos.y < this->_centipede[i].pos.y && this->_playerPos.y + 1 > this->_centipede[i].pos.y) {
                this->_playerAlive = false;
                this->updateHighScores();
            }
        }
    }

    // Draw player
    if (this->_playerAlive) {
        this->_gameDisplay.sprites.push_back({"player", {this->_playerPos.x, this->_playerPos.y}, 0, false, false, true});
    }

    // Draw missile
    if (this->_missileExists) {
        this->_gameDisplay.sprites.push_back({"missile", {this->_missilePos.x, this->_missilePos.y}, 0, false, false, true});
    }

    // Draw centipede
    for (std::size_t i = 0; i != this->_centipede.size(); i++) {
        if (this->_centipede[i].isDead) {
            continue;
        }
        if (!this->_centipede[i].isLinked) {
            this->_gameDisplay.sprites.push_back({"centipede_head", {this->_centipede[i].pos.x, this->_centipede[i].pos.y}, this->_centipede[i].rotate, false, false, true});
        } else {
            if (i + 1 != this->_centipede.size() && !this->_centipede[i + 1].isDead && this->_centipede[i - 1].pos.y != this->_centipede[i].pos.y && this->_centipede[i + 1].pos.y != this->_centipede[i].pos.y && this->_centipede[i - 1].pos.y != this->_centipede[i + 1].pos.y) {
                this->_centipede[i].rotate = 0;
                this->_gameDisplay.sprites.push_back({"centipede_body", {this->_centipede[i].pos.x, this->_centipede[i].pos.y}, this->_centipede[i].rotate, false, false, true});
            } else if (i + 1 != this->_centipede.size() && !this->_centipede[i + 1].isDead && this->_centipede[i - 1].pos.y != this->_centipede[i].pos.y) {
                if (this->_centipede[i].centipedeVelocity.x == -1 && this->_centipede[i - 1].centipedeVelocity.x == 1) {
                    this->_centipede[i].rotate = 90;
                }
                if (this->_centipede[i].centipedeVelocity.x == 1 && this->_centipede[i - 1].centipedeVelocity.x == -1) {
                    this->_centipede[i].rotate = 180;
                }
                this->_gameDisplay.sprites.push_back({"centipede_turn", {this->_centipede[i].pos.x, this->_centipede[i].pos.y}, this->_centipede[i].rotate, false, false, true});
            } else if (i + 1 != this->_centipede.size() && !this->_centipede[i + 1].isDead && this->_centipede[i + 1].pos.y != this->_centipede[i].pos.y) {
                if (this->_centipede[i].centipedeVelocity.x == -1 && this->_centipede[i + 1].centipedeVelocity.x == 1) {
                    this->_centipede[i].rotate = 270;
                }
                if (this->_centipede[i].centipedeVelocity.x == 1 && this->_centipede[i + 1].centipedeVelocity.x == -1) {
                    this->_centipede[i].rotate = 0;
                }
                this->_gameDisplay.sprites.push_back({"centipede_turn", {this->_centipede[i].pos.x, this->_centipede[i].pos.y}, this->_centipede[i].rotate, false, false, true});
            } else {
                if (i + 1 == this->_centipede.size() || this->_centipede[i + 1].isDead) {
                    if (this->_centipede[i].pos.y != this->_centipede[i - 1].pos.y) {
                        this->_centipede[i].rotate = 0;
                    } else {
                        this->_centipede[i].rotate = 90;
                    }
                }
                this->_gameDisplay.sprites.push_back({"centipede_body", {this->_centipede[i].pos.x, this->_centipede[i].pos.y}, this->_centipede[i].rotate, false, false, true});
            }
        }
    }

    // Draw obstacles
    for (std::size_t y = 0; y != this->_obstacles.size(); y++) {
        for (std::size_t x = 0; x != this->_obstacles[y].size(); x++) {
            if (this->_obstacles[y][x] != 0) {
                this->_gameDisplay.sprites.push_back({"obstacle" + std::to_string(this->_obstacles[y][x]), {(float)x, (float)y + TOP_SIZE}, 0, false, false, true});
            }
        }
    }
}

void Centipede::gameOverUpdate([[maybe_unused]]double delta)
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
    this->_gameDisplay.tiles.drawText(3, 0 + TOP_SIZE, "GAME OVER", this->_charTable);
    this->_gameDisplay.tiles.drawText(3, 1 + TOP_SIZE, "SCORE: " + std::to_string(this->_score), this->_charTable);
    this->_gameDisplay.tiles.drawText(3, 3 + TOP_SIZE, "HIGHSCORES:", this->_charTable);
    for (; i != this->_highScores.size(); i++) {
        this->_gameDisplay.tiles.drawText(3, 4 + TOP_SIZE + i, this->_highScoresNames[i] + ": " + std::to_string(this->_highScores[i]), this->_charTable);
    }
    this->_gameDisplay.tiles.drawText(4, 5 + i + TOP_SIZE, "RESTART", this->_charTable);
    this->_gameDisplay.tiles.drawText(4, 6 + i + TOP_SIZE, "MENU", this->_charTable);
    this->_gameDisplay.tiles.drawText(3, 5 + i + TOP_SIZE + this->_menuSelection, ">", this->_charTable);
}

void Centipede::onUpdate(double delta)
{
    this->_gameDisplay.tiles.drawText(11, 0, "CENTIPEDE", this->_charTable);
    this->_gameDisplay.tiles.drawText(5, 1, "Time: " + std::to_string((int)this->_timer / 1000), this->_charTable);
    this->_gameDisplay.tiles.drawText(5, 2, "Score: " + std::to_string(this->_score), this->_charTable);
    if (this->_playerAlive) {
        this->gameUpdate(delta);
    } else {
        this->gameOverUpdate(delta);
    }
}

void Centipede::onEvent(Arcade::Event ev)
{
    if (ev.type == Arcade::Event::Type::KEY_DOWN)
    {
        if (ev.data.keyboard.key == 'z' || ev.data.keyboard.key == 'R')
        {
            if (!this->_playerAlive) {
                this->_menuSelection = (this->_menuSelection + 1) % 2;
            } else {
                this->_playerVelocity.y = -1;
            }
        }
        if (ev.data.keyboard.key == 's' || ev.data.keyboard.key == 'Q')
        {
            if (!this->_playerAlive) {
                this->_menuSelection = (this->_menuSelection + 1) % 2;
            } else {
                this->_playerVelocity.y = 1;
            }
        }
        if (ev.data.keyboard.key == 'q' || ev.data.keyboard.key == 'P')
        {
            this->_playerVelocity.x = -1;
        }
        if (ev.data.keyboard.key == 'd' || ev.data.keyboard.key == 'O')
        {
            this->_playerVelocity.x = 1;
        }
        if (ev.data.keyboard.key == ' ')
        {
            if (!this->_missileExists) {
                this->_missileExists = true;
                this->_missilePos.x = this->_playerPos.x;
                this->_missilePos.y = this->_playerPos.y + 1;
            }
        }
        if (ev.data.keyboard.key == '\r') {
            if (!this->_playerAlive) {
                if (this->_menuSelection == 0) {
                    this->initGame();
                } else {
                    this->_gameDisplay.music = "";
                    this->arcade->setGame("Menu");
                }
            }
        }
    }
    if (ev.type == Arcade::Event::Type::KEY_UP)
    {
        if (ev.data.keyboard.key == 'z' || ev.data.keyboard.key == 'R')
        {
            if (this->_playerVelocity.y == -1) {
                this->_playerVelocity.y = 0;
            }
        }
        if (ev.data.keyboard.key == 's' || ev.data.keyboard.key == 'Q')
        {
            if (this->_playerVelocity.y == 1) {
                this->_playerVelocity.y = 0;
            }
        }
        if (ev.data.keyboard.key == 'q' || ev.data.keyboard.key == 'P')
        {
            if (this->_playerVelocity.x == -1) {
                this->_playerVelocity.x = 0;
            }
        }
        if (ev.data.keyboard.key == 'd' || ev.data.keyboard.key == 'O')
        {
            if (this->_playerVelocity.x == 1) {
                this->_playerVelocity.x = 0;
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
        return "Centipede";
    }

    Arcade::IGameModule *getInstance(Arcade::IArcade* arcade) {
        return new Centipede(arcade);
    }

    void deleteInstance(Arcade::IGameModule *instance) {
        delete instance;
    }
}
