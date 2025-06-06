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
    if (currentRound % roundPerAction != 0 || roundPerAction > 50)
        return;
    cout << " >> Guardian '" << name << "' activates skill " << endl;
    switch (id)
    {
    case 0:
        ally.heal(ally.stats.maxHp * 0.12);
        break;
    case 1:
        ally.energy.current += 30;
        break;
    case 2:
        ally.applyStatBuff(StatType::DEF, 0.24 * ally.stats.maxDef, 3, name);
        break;
    case 3:
        ally.applyDot(enemy,5,0.4);
        break;
    case 4:
        ally.sStats.dmgBonus += 0.1;
        break;
    default:
        break;
    }
}
