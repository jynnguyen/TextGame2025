#include "Orb.hpp"

Orb::Orb(string n , BaseStats b, CritStats cr, Modifiers m, Effect res, Effect hr, Agility agi) :
name(n),base(b),crit(cr),mod(m),resistance(res),hitRate(hr),agility(agi)
{
}

void Orb::setOwned()
{
    owned = true;
}

void Orb::info()
{
    cout << string(2, '-') << name << " (Rarity: " << rarity << " - Owned: " << (owned ? "Yes" : "No");
    cout << ")" << string(2, '-') << endl;
    base.info();
    crit.info();
    mod.info();
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