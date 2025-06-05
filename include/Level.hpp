#pragma once
#include "main.hpp"

class Level
{
public:
    Level(int lv = 1, int lvCap=100) : level(lv), levelCap(lvCap){
        setRequiredExp();
    }

    int get();
    void set(int lv = 1);
    void setCurrentExp(int amount = 0);
    int getCurrentExp();
    bool up();
    void info();

private:
    int level, levelCap;
    int currentExp = 0;
    int requiredExp;

    void setRequiredExp();
    bool hasEnoughExp();
};