#include "Orb.hpp"

Orb::Orb(string n, BaseStats b, CritStats cr, Modifiers m, Effect res, Effect hr, Agility agi) : name(n), base(b), crit(cr), mod(m), resistance(res), hitRate(hr), agility(agi)
{
}

void Orb::setOwned()
{
    owned = true;
}

string Orb::info()
{
    stringstream ss;
    ss << string(2, '*') << name << " (Rarity: " << rarity << " - Owned: " << (owned ? "Yes" : "No");
    ss << ")" << string(2, '*') << endl;
    ss << base.info();
    ss << crit.info();
    ss << mod.info();
    if (!resistance.info().empty())
        ss << " - Resistance: " << resistance.info();
    if (!hitRate.info().empty())
        ss << " - Hit Rate: " << hitRate.info();
    ss << agility.info();
    return ss.str();
}

bool Orb::getOwned() const
{
    return owned;
}

string Orb::getName() const
{
    return name;
}

int Orb::getId() const
{
    return id;
}
void Orb::setID(int i)
{
    id = i;
}

void Orb::setRarity(string r)
{
    rarity = r;
}

string Orb::getRarity() const
{
    return rarity;
}