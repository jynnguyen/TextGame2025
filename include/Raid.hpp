#pragma once
#include "main.hpp"
#include "Unit.hpp"
#include "Guardian.hpp"
#include "Functions.hpp"

class Raid
{
public:
    Raid(Unit deploy = Unit(0), Unit e = Unit(1), Guardian g = Guardian(), double s = 1);
    bool normal();
    double boss();
    void takeAction(Unit &attacker, Unit &target);

private:
    Unit onDeploy;
    Unit enemy;
    Guardian guardian;
    double speed = 1;
};