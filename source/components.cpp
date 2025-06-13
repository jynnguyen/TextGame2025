#include "Components.hpp"

void BaseStats::reset()
{
    hp = maxHp;
    atk = maxAtk;
    def = maxDef;
}

void BaseStats::info()
{
    cout << "HP: " << hp << " | ATK: " << atk << " | DEF: " << def << endl;
}

BaseStats& BaseStats::operator*=(double num)
{
    maxAtk *= num;
    maxHp *= num;
    maxDef *= num;
    return *this;
}

BaseStats BaseStats::multiply(double num) const
{
    BaseStats result = *this;
    result.atk *= num;
    result.hp *= num;
    result.def *= num;
    return result;
}

void Energy::reset()
{
    max = base_max;
    current = 0;
    regen = base_regen;
}

void Energy::info()
{
    cout << "Energy: " << current << "/" << max << endl;
}

void CritStats::reset()
{
    rate = base_rate;
    dmg = base_dmg;
}

void CritStats::info()
{
    cout << "Crit Rate: " << rate * 100 << "% | Crit Dmg: " << dmg * 100 << "%" << endl;
}

void Status::reset()
{
    is = false, duration = -1;
    scale = 0;
}

void Modifiers::reset()
{
    penetration = 0;
    dmgBonus = 0;
}

void Modifiers::info()
{
    if (dmgBonus > 0)
        cout << "Dmg Bonus: " << dmgBonus * 100 << "% | ";
    if (penetration > 0)
        cout << "Penetration: " << penetration * 100 << "% | ";
    if (ultDmgBonus > 0)
        cout << "Ult Dmg Bonus: " << ultDmgBonus * 100 << "% | ";
    if (dotDmgBonus > 0)
        cout << "Dot Dmg Bonus: " << dotDmgBonus * 100 << "% | ";
    if (penetration > 0 || ultDmgBonus > 0 || dmgBonus > 0 || dotDmgBonus > 0)
        cout << endl;
    cout << endl;
}

BaseStats BaseStats::operator+(const BaseStats& other) const
{
    return BaseStats(hp + other.hp, atk + other.atk, def + other.def);
}

BaseStats& BaseStats::operator+=(const BaseStats& other)
{
    maxHp += other.maxHp;
    maxAtk += other.maxAtk;
    maxDef += other.maxDef;
    return *this;
}

BaseStats BaseStats::operator-(const BaseStats& other) const
{
    return BaseStats(hp - other.hp, atk - other.atk, def - other.def);
}

BaseStats& BaseStats::operator=(const BaseStats& other)
{
    if (this != &other)
    {
        hp = other.hp;
        atk = other.atk;
        def = other.def;
        base_hp = other.base_hp;
        base_atk = other.base_atk;
        base_def = other.base_def;
        maxHp = other.maxHp;
        maxAtk = other.maxAtk;
        maxDef = other.maxDef;
    }
    return *this;
}

CritStats& CritStats::operator+=(const CritStats& other)
{
    rate += other.rate;
    dmg += other.dmg;
    return *this;
}

CritStats& CritStats::operator-=(const CritStats& other)
{
    rate -= other.rate;
    dmg -= other.dmg;
    return *this;
}

CritStats& CritStats::operator=(const CritStats& other)
{
    if (this != &other)
    {
        rate = other.rate;
        dmg = other.dmg;
    }
    return *this;
}

Modifiers& Modifiers::operator+=(const Modifiers& other)
{
    penetration += other.penetration;
    ultDmgBonus += other.ultDmgBonus;
    dmgBonus += other.dmgBonus;
    dotDmgBonus += other.dotDmgBonus;
    return *this;
}

Modifiers& Modifiers::operator-=(const Modifiers& other)
{
    penetration -= other.penetration;
    ultDmgBonus -= other.ultDmgBonus;
    dmgBonus -= other.dmgBonus;
    dotDmgBonus -= other.dotDmgBonus;
    return *this;
}

Modifiers& Modifiers::operator=(const Modifiers& other)
{
    if (this != &other)
    {
        penetration = other.penetration;
        ultDmgBonus = other.ultDmgBonus;
        dmgBonus = other.dmgBonus;
        dotDmgBonus = other.dotDmgBonus;
    }
    return *this;
}

Effect& Effect::operator+=(const Effect& other)
{
    dot += other.dot;
    cc += other.cc;
    dmg += other.dmg;
    return *this;
}

Agility& Agility::operator+=(const Agility& other){
    evade += other.evade;
    accuracy += other.accuracy;
    return *this;
}