#pragma once
#include "Components.hpp"
#include "Orb.hpp"
#include "StatsCal.hpp"
#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

class Unit
{
public:
    StatsCal stats;
    Energy energy;
    Status crowdControl;
    Orb orb;

    vector<Status> dots;

    Unit(int t, string n = "", BaseStats b = {1, 1, 1}, Energy e = 1000);

    void applyDot(Unit& target, int duration, double scale = 0);
    void applyCC(Unit& target, int duration);
    bool isDotted(Unit& target);
    bool isCced();
    bool updateBadStatus(Unit& dotDmgDealer);

    double attack(Unit& target, double scale = 1, double energyRegen = 1, StatsCal::BaseType scaleOn = StatsCal::BaseType::ATK);
    void ultimate(Unit& target);
    double dotAttack(Unit& dotDmgDealer, const Status &dot);
    double trueAttack(Unit& target, double dmg);

    void info();
    void displayStats();
    bool isAlive();
    bool isEnoughEnergy();
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
    bool isEvaded(Unit& other);

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

    double dmgCal(const Unit& target, double scale = 1, StatsCal::BaseType scaleOn = StatsCal::BaseType::ATK);
};
