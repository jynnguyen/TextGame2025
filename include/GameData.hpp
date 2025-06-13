#pragma once
#include "main.hpp"
#include "Unit.hpp"
#include "Level.hpp"
#include "Configuration.hpp"
#include "Functions.hpp"

class GameData
{
public:
    GameData(const string& fileName);
    Unit spawnEnemy(bool isBoss = false);
    void save();
    void inventory(string type = "");
    void setUnitLevel();

private:
    friend class Game;
    int gold = 50, ruby = 10;
    int speed = 1;
    Level level;
    Configuration config;
    vector<Unit> units;
    vector<Unit> enemies;
    vector<Orb> orbs;
    vector<Guardian> guardians;
};