#pragma once
#include "main.hpp"
using namespace std;

class Components
{
public:
    bool has = false;
    virtual string info() = 0;
    string formatDouble(const double& value, int precision = 2);
};

class BaseStats : public Components
{
public:
    double hp, maxHp,
        atk, maxAtk,
        def, maxDef;
    BaseStats(double h = 0, double a = 0, double d = 0) : hp(h), atk(a), def(d), base_hp(h), base_atk(a), base_def(d), maxHp(h), maxAtk(a), maxDef(d) {}
    void reset();
    string info() override;
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
    string info() override;

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
    string info() override;

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
    double dmgBonus = 0;
    double penetration = 0;
    double ultDmgBonus = 0;
    double dotDmgBonus = 0;

    Modifiers() = default;
    Modifiers(double bon, double pen, double ultBon, double dotBon)
        : penetration(pen), dmgBonus(bon), ultDmgBonus(ultBon), dotDmgBonus(dotBon) {}
    void reset();
    string info() override;
    Modifiers &operator+=(const Modifiers &other);
    Modifiers &operator-=(const Modifiers &other);
    Modifiers &operator=(const Modifiers &other);
};

class Effect : public Components
{
public:
    double dot = 0;
    double cc = 0;
    double dmg = 0;
    Effect(double dm = 0, double c = 0, double dt = 0) : dmg(dm), cc(c), dot(dt) {}
    Effect &operator+=(const Effect &other);
    string info() override;
    void reset();
};

class Agility : public Components
{
public:
    double evade = 0;
    double accuracy = 0;
    Agility(double e = 0, double ac = 0) : evade(e), accuracy(ac) {}
    Agility &operator+=(const Agility &other);
    string info() override;
    void reset();
};