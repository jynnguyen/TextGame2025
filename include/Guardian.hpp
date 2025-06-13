#pragma once
#include "main.hpp"
#include "Unit.hpp"

class Guardian
{
public:
    Guardian(string n = "", int i = -1, int rPA = 0):name(n),id(i),roundPerAction(rPA){}
    string info();
    void skill(const int& currentRound,Unit& ally, Unit& enemy);
    bool getOwned() const;
    void setOwned();
    string getName() const;
    int getId() const;
private:
    string name;
    int id, roundPerAction;
    bool owned = false;
};