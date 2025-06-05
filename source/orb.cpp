#include "Orb.hpp"

void Orb::setOwned()
{
    owned = true;
}

void Orb::info()
{
    cout << string(2, '-') << name << " (Rarity: " << rarity << " - Owned: " << (owned ? "Yes" : "No");
    cout << ")" << string(2, '-') << endl;
    stats.info();
    critStats.info();
    sStats.info();
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