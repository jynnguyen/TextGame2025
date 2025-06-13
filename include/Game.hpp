#pragma once
#include "Unit.hpp"
#include "Raid.hpp"
#include "Gacha.hpp"
#include "GameData.hpp"
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

class Game
{

public:
    Game(const string &fileName);
    ~Game();
    void run();

private:
    GameData *gameData;
    int unitIdx = -1, gIdx = -1;
    string playerInput;
    bool gameRunning = true;

    // Game Scenes
    void handleInput();
    void menu();
    void raid();
    void boss();

    // Game Features
    void deployUnit();
    void selectGuardian();
    void equipOrb();
    void inventory();
    void displayInfo();
    void gacha();
    void levelUp();
    void setRaidSpeed();

    // Admin
    void adminMode();
    void reloadData();
};
