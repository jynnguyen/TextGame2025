#pragma once

#include "main.hpp"
#include "Components.hpp"

class Orb
{
public:
    Orb(string n = "", BaseStats b = {0, 0, 0}, CritStats cr = {0, 0}, Modifiers m = {0, 0, 0, 0}, Effect res = {0, 0, 0}, Effect hr = {0, 0, 0}, Agility agi = {0, 0});
    void setOwned();
    bool getOwned() const;
    string getName() const;
    int getId() const;
    void setID(int i);
    void info();
    void setRarity(string r);
    string getRarity() const;

protected:
    string name, rarity = "SR";
    bool owned = 0;
    BaseStats base;
    CritStats crit;
    Modifiers mod;
    Effect resistance;
    Effect hitRate;
    Agility agility;
    int id = -1;
    friend class Unit;
    friend class Game;
};
