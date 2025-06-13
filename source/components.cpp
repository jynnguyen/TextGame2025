#include "Components.hpp"

string Components::formatDouble(const double &value, int precision)
{
    ostringstream oss;
    oss << fixed << setprecision(precision) << value;
    return oss.str();
}

void BaseStats::reset()
{
    hp = maxHp;
    atk = maxAtk;
    def = maxDef;
}

string BaseStats::info()
{
    stringstream ss;
    ss << " - HP: " << formatDouble(hp) << " | ATK: " << formatDouble(atk) << " | DEF: " << formatDouble(def) << endl;
    return ss.str();
}

BaseStats &BaseStats::operator*=(double num)
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

string Energy::info()
{
    stringstream ss;
    ss << " - Energy: " << formatDouble(current) << "/" << formatDouble(max) << endl;
    return ss.str();
}

void CritStats::reset()
{
    rate = base_rate;
    dmg = base_dmg;
}

string CritStats::info()
{
    stringstream ss;
    ss << " - Crit Rate: " << formatDouble(rate * 100) << "% | Crit Dmg: " << formatDouble(dmg * 100) << "%" << endl;
    return ss.str();
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

string Modifiers::info()
{
    stringstream ss;
    if (penetration > 1e-10 || ultDmgBonus > 1e-10 || dmgBonus > 1e-10 || dotDmgBonus > 1e-10)
        ss << " - ";
    if (dmgBonus > 1e-10)
        ss << "Dmg Bonus: " << formatDouble(dmgBonus * 100) << "% | ";
    if (penetration > 1e-10)
        ss << "Penetration: " << formatDouble(penetration * 100) << "% | ";
    if (ultDmgBonus > 1e-10)
        ss << "Ult Dmg Bonus: " << formatDouble(ultDmgBonus * 100) << "% | ";
    if (dotDmgBonus > 1e-10)
        ss << "Dot Dmg Bonus: " << formatDouble(dotDmgBonus * 100) << "% | ";
    if (penetration > 1e-10 || ultDmgBonus > 1e-10 || dmgBonus > 1e-10 || dotDmgBonus > 1e-10)
        ss << endl;
    return ss.str();
}

BaseStats BaseStats::operator+(const BaseStats &other) const
{
    return BaseStats(hp + other.hp, atk + other.atk, def + other.def);
}

BaseStats &BaseStats::operator+=(const BaseStats &other)
{
    maxHp += other.maxHp;
    maxAtk += other.maxAtk;
    maxDef += other.maxDef;
    return *this;
}

BaseStats BaseStats::operator-(const BaseStats &other) const
{
    return BaseStats(hp - other.hp, atk - other.atk, def - other.def);
}

BaseStats &BaseStats::operator=(const BaseStats &other)
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

CritStats &CritStats::operator+=(const CritStats &other)
{
    rate += other.rate;
    dmg += other.dmg;
    return *this;
}

CritStats &CritStats::operator-=(const CritStats &other)
{
    rate -= other.rate;
    dmg -= other.dmg;
    return *this;
}

CritStats &CritStats::operator=(const CritStats &other)
{
    if (this != &other)
    {
        rate = other.rate;
        dmg = other.dmg;
    }
    return *this;
}

Modifiers &Modifiers::operator+=(const Modifiers &other)
{
    penetration += other.penetration;
    ultDmgBonus += other.ultDmgBonus;
    dmgBonus += other.dmgBonus;
    dotDmgBonus += other.dotDmgBonus;
    return *this;
}

Modifiers &Modifiers::operator-=(const Modifiers &other)
{
    penetration -= other.penetration;
    ultDmgBonus -= other.ultDmgBonus;
    dmgBonus -= other.dmgBonus;
    dotDmgBonus -= other.dotDmgBonus;
    return *this;
}

Modifiers &Modifiers::operator=(const Modifiers &other)
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

Effect &Effect::operator+=(const Effect &other)
{
    dot += other.dot;
    cc += other.cc;
    dmg += other.dmg;
    return *this;
}

string Effect::info()
{
    stringstream ss;
    if (cc > 1e-10)
        ss << "Crowd Control: " << formatDouble(cc * 100) << "%" << " | ";
    if (dot > 1e-10)
        ss << "DOT: " << formatDouble(dot * 100) << "%" << " | ";
    if (dmg > 1e-10)
        ss << "Dmg: " << formatDouble(dmg * 100) << "%";
    if (cc > 1e-10 || dot > 1e-10 || dmg > 1e-10)
        ss << endl;
    return ss.str();
}

Agility &Agility::operator+=(const Agility &other)
{
    evade += other.evade;
    accuracy += other.accuracy;
    return *this;
}

string Agility::info()
{
    stringstream ss;
    if (evade > 1e-10 || accuracy > 1e-10)
        ss << " - ";
    if (evade > 1e-10)
        ss << "Evade: " << formatDouble(evade * 100) << "%" << " | ";
    if (accuracy > 1e-10)
        ss << "Accuracy: " << formatDouble(accuracy * 100) << "%" << " | ";
    if (evade > 1e-10 || accuracy > 1e-10)
        ss << endl;
    return ss.str();
}