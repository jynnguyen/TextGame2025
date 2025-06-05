#pragma once

#include "main.hpp"
#include "Unit.hpp"
#include "Orb.hpp"
#include "Guardian.hpp"

class Configuration
{
public:
    Configuration(const string &fileName);
    void loadAsset(const string &str);
    void loadUnit(const string &str, string type);
    void loadOrb(const string &str);
    void loadGuardian(const string &str);
    void loadSave(const string &str);
    void setUnitLevel();
    void clearAll();
    void saveAll();
    string getDataPath() const;

private:
    string saveFile = "Data/save.txt";
    string dataFile;
    vector<Unit> units;
    vector<Unit> enemies;
    vector<Orb> orbs;
    vector<Guardian> guardians;
    int gold = 100, ruby = 10, level = 1, currentExp = 0;
    friend class GameData;
};