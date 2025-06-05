#include "Guardian.hpp"

void Guardian::info()
{
    cout << string(2, '-') << name << " (Owned: " << (owned ? "Yes" : "No");
    cout << ")" << string(2, '-') << endl;
}
bool Guardian::getOwned() const
{
    return owned;
}

string Guardian::getName() const
{
    return name;
}

int Guardian::getId() const
{
    return id;
}

void Guardian::setOwned()
{
    owned = true;
}

void Guardian::skill(const int &currentRound, Unit &ally, Unit &enemy)
{
    if (currentRound % roundPerAction != 0 && roundPerAction < 50)
        return;
    cout << " >> Guardian '" << name << "' activates skill " << endl;
    switch (id)
    {
    case 0:
        ally.heal(ally.stats.maxHp * 0.08);
        break;
    case 1:
        ally.energy.current += 25;
        break;
    case 2:
        ally.applyStatBuff(StatType::DEF, 0.2 * ally.stats.maxDef, 3, name);
    default:
        return;
    }
}
