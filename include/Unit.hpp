#pragma once
#include "Components.hpp"
#include "Orb.hpp"
#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

class Unit
{
public:
    Stats stats;
    Energy energy;
    CritStats critStats;
    Status crowdControl;
    SpecialStats sStats;
    Orb orb;

    vector<TimeModifier<StatType>> statBuffs;
    vector<TimeModifier<CritType>> critBuffs;
    vector<TimeModifier<SpecialStatType>> sStatBuffs;
    vector<Status> dots;
    void applyStatBuff(StatType type, double value, int duration, string source, bool stackable = false);
    void applyCritBuff(CritType type, double value, int duration, string source, bool stackable = false);
    void applySpecialBuff(SpecialStatType type, double value, int duration, string source, bool stackable = false);
    void updateBuffs();

    Unit(string n = "", Stats s = {1, 1, 1}, Energy e = 1000) : name(n), stats(s), energy(e)
    {
        resetToBase();
    }

    void applyDot(Unit &target, int duration, double scale = 0);
    void applyCC(Unit &target, int duration);
    bool isDotted(Unit &target);
    bool isCced();
    bool updateBadStatus(Unit &dotDmgDealer);

    double attack(Unit &target, double scale = 1, double energyRegen = 1);
    void ultimate(Unit &target);
    double dotAttack(Unit &dotDmgDealer,const Status& dot);
    double trueAttack(Unit &target, double dmg);
    double sAttack(Unit &target, double &dmg);

    void info();
    void displayStats();
    bool isAlive();
    bool isEnoughEnergy();
    void reCalStats();
    void resetToBase();
    void applyOrb();
    void setID(int i);
    void setType(int t);
    string getName() const;
    int getLevel() const;
    void setLevel(int lv);
    int getId() const;
    int getType() const;
    bool getOwned() const;
    void setOwned();
    bool isEvaded();

    void heal(double amount);
    void setRarity(string r);
    string getRarity();

    void update();

private:
    string rarity = "SR";
    string name = "No";
    bool owned = false;
    int level = 0;
    int id = -1, type = -1;

    double dmgCal(const Unit &target, double scale = 1);
    bool isCrit();
    void applyStatDelta(StatType type, double delta);
    void applyCritDelta(CritType type, double delta);
    void applySpecialStatDelta(SpecialStatType type, double delta);
};
