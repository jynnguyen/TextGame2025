#pragma once
#include "main.hpp"
#include "Unit.hpp"
#include "Guardian.hpp"
#include "Functions.hpp"

class Raid{ 
    public:
    Raid(Unit deploy = Unit(), Unit e = Unit(),Guardian g = Guardian(), double s = 1):onDeploy(deploy),enemy(e),guardian(g),speed(s){}
    bool normal();
    double boss();
    void takeAction(Unit& attacker, Unit& target);

    private:
    Unit onDeploy;
    Unit enemy;
    Guardian guardian;
    double speed = 1;
};