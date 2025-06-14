#include "Guardian.hpp"

string Guardian::info()
{
    stringstream ss;
    ss << string(2, '-') << name << " (Owned: " << (owned ? "Yes" : "No");
    ss << ")" << string(2, '-') << endl;
    ss << "Rounds Per Action: "<<roundPerAction<<endl;
    return ss.str();
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
        ally.stats.buffBase(StatsCal::BaseType::DEF, 0.4 * ally.stats.base.maxDef, 3, name);
        break;
    case 3:
        ally.applyDot(enemy,5,0.3);
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
