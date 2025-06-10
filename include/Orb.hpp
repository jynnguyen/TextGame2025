#pragma once

#include "main.hpp"
#include "Components.hpp"

class Orb
{
public:
    Orb(string n = "", BaseStats b = {0, 0, 0}, CritStats c = {0, 0}, Modifiers m = {0, 0, 0, 0}) : name(n), base(b), crit(c), mod(m)
    {
    }
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
    int id = -1;
    friend class Unit;
    friend class Game;
};
