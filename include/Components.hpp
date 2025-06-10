#pragma once
#include <cmath>
#include <iostream>
using namespace std;

class Components
{
public:
    bool has = false;
    virtual void info() = 0;
};

class BaseStats : public Components
{
public:
    double hp, maxHp,
        atk, maxAtk,
        def, maxDef;
    BaseStats(double h, double a, double d) : hp(h), atk(a), def(d), base_hp(h), base_atk(a), base_def(d), maxHp(h), maxAtk(a), maxDef(d) {}
    void reset();
    void info() override;
    BaseStats multiply(double num) const;
    BaseStats operator+(const BaseStats &other) const;
    BaseStats &operator*=(double num);
    BaseStats &operator+=(const BaseStats &other);
    BaseStats operator-(const BaseStats &other) const;
    BaseStats &operator=(const BaseStats &other);

    double base_hp,
        base_atk,
        base_def;
};

class Energy : public Components
{
public:
    double base_max = 1000, max = 1000;
    double regen = 25;
    double current = 0;
    Energy(double m) : max(m), base_max(m)
    {
    }
    void reset();
    void info() override;

private:
    double base_regen = 25;
};

class CritStats : public Components
{
public:
    double rate = 0.05, dmg = 0.5;
    CritStats() = default;
    CritStats(double r, double d) : rate(r), dmg(d), base_rate(r), base_dmg(d) {}
    void reset();
    void info() override;

    CritStats &operator+=(const CritStats &other);
    CritStats &operator-=(const CritStats &other);
    CritStats &operator=(const CritStats &other);

private:
    double base_rate = 0.05, base_dmg = 0.5;
};

class Status
{
public:
    bool is;
    int duration;
    double scale;
    Status(bool i = false, int d = -1, double sc = 0) : is(i), duration(d), scale(sc) {}
    void reset();
};

class Modifiers : public Components
{
public:
    double penetration = 0;
    double dmgReduction = 0;
    double dmgBonus = 0;
    double evade = 0.01;
    Modifiers() = default;
    Modifiers(double pen, double red, double bon, double ev)
        : penetration(pen), dmgReduction(red), dmgBonus(bon), evade(ev) {}
    void reset();
    void info() override;
    Modifiers &operator+=(const Modifiers &other);
    Modifiers &operator-=(const Modifiers &other);
    Modifiers &operator=(const Modifiers &other);
};

