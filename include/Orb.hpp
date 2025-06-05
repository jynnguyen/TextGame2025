#pragma once

#include "main.hpp"
#include "Components.hpp"

class Orb
{
public:
    Orb(string n = "", Stats s = {0, 0, 0}, CritStats c = {0, 0}, SpecialStats ss = {0, 0, 0, 0}) : name(n), stats(s), critStats(c), sStats(ss)
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
    Stats stats;
    CritStats critStats;
    SpecialStats sStats;
    int id = -1;
    friend class Unit;
    friend class Game;
};
