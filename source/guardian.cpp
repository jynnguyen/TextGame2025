#include "Guardian.hpp"

void Guardian::info()
{
    cout << string(2, '-') << name << " (Owned: " << (owned ? "Yes" : "No");
    cout << ")" << string(2, '-') << endl;
    cout << "Rounds Per Action: "<<roundPerAction<<endl;
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
    if (roundPerAction == 0 || currentRound % roundPerAction != 0)
        return;
    cout << " >> Guardian '" << name << "' activates skill " << endl;
    switch (id)
    {
    case 0:
        ally.heal(ally.stats.base.maxHp * 0.12);
        break;
    case 1:
        ally.energy.current += 30;
        break;
    case 2:
        ally.stats.buffBase(StatsCal::BaseType::DEF, 0.5 * ally.stats.base.maxDef, 4, name);
        break;
    case 3:
        ally.applyDot(enemy,10,0.1);
        break;
    case 4:
        ally.stats.mod.dmgBonus += 0.08;
        break;
    case 5:
        ally.applyCC(enemy,1);
        break;
    default:
        break;
    }
}
