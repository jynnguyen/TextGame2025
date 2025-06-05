#include "Unit.hpp"
#include "Functions.hpp"

void Unit::info()
{
    cout << string(2, '-') << name << " (Rarity: " << rarity << " - Owned: " << (owned ? "Yes" : "No");
    cout << ")" << string(2, '-') << endl;
    stats.info();
    critStats.info();
    sStats.info();
}

void Unit::displayStats()
{
    cout << " <" << name << "> - Energy: (" << energy.current << "/" << energy.max << ")" << endl;
    stats.info(), critStats.info(), sStats.info();
}

string Unit::getName() const
{
    return name;
}

int Unit::getId() const
{
    return id;
}
int Unit::getType() const
{
    return type;
}

void Unit::setType(int t)
{
    type = t;
}

int Unit::getLevel() const
{
    return level;
}

bool Unit::getOwned() const
{
    return owned;
}

void Unit::setOwned()
{
    owned = true;
}

bool Unit::isAlive()
{
    if (stats.hp < 0)
        stats.hp = 0;
    return stats.hp > 0;
}

bool Unit::isEnoughEnergy()
{
    return energy.current >= energy.max;
}

bool Unit::isEvaded()
{
    double noEvade = (1 - sStats.evade) < 0 ? 0 : 1 - sStats.evade;
    return rngRate(noEvade, sStats.evade) == 1;
}

void Unit::setID(int i)
{
    id = i;
}

void Unit::heal(double amount)
{
    cout << " > " << name << " heals " << amount << "_hp" << endl;
    stats.hp += amount;
    stats.hp = (stats.hp > stats.maxHp) ? stats.maxHp : stats.hp;
}

void Unit::setRarity(string r)
{
    rarity = r;
}

string Unit::getRarity()
{
    return rarity;
}

void Unit::reCalStats()
{
    stats.reset();
    energy.reset();
    critStats.reset();
    crowdControl.reset();
    sStats.reset();
    statBuffs.clear(), sStatBuffs.clear(), critBuffs.clear(), dots.clear();
}

bool Unit::isCrit()
{
    return rngRate(1 - critStats.rate, critStats.rate) == 1;
}

double Unit::dmgCal(const Unit &target, double scale)
{
    double finalDmg = (stats.atk * scale - target.stats.def * (1 - sStats.penetration));
    double dmgReduce = (1 - target.sStats.dmgReduction) < 0.01 ? 0.01 : 1 - target.sStats.dmgReduction;
    finalDmg = (finalDmg < 1) ? 1 : finalDmg;
    if (isCrit())
    {
        cout << " > Critical Hit" << endl;
        finalDmg = finalDmg * (1 + critStats.dmg);
    }
    finalDmg = finalDmg * (1 + sStats.dmgBonus) * dmgReduce;
    return (finalDmg < 1) ? 1 : finalDmg;
}

double Unit::attack(Unit &target, double scale, double energyRegen)
{
    if (target.isEvaded())
    {
        cout << " > " << target.name << " evades the attack" << endl;
        return 0;
    }
    double dmg = dmgCal(target, scale);
    cout << " > " << name << " dealt " << dmg << "_dmg to " << target.name << endl;
    target.stats.hp -= dmg;
    energy.current += energy.regen;
    target.energy.current += target.energy.regen * energyRegen / 5;
    if (!target.isAlive())
        cout << " >> " << target.name << " is dead !" << endl;
    return dmg;
}

double Unit::sAttack(Unit &target, double &dmg)
{
    dmg -= target.stats.def * (1 - sStats.penetration);
    double dmgReduce = (1 - target.sStats.dmgReduction) < 0.01 ? 0.01 : 1 - target.sStats.dmgReduction;
    dmg = (dmg < 1) ? 1 : dmg;
    if (isCrit())
    {
        cout << " > Critical Hit" << endl;
        dmg = dmg * (1 + critStats.dmg);
    }
    dmg = dmg * (1 + sStats.dmgBonus) * dmgReduce;
    if (target.isEvaded())
    {
        cout << " > " << target.name << " evades the attack" << endl;
        return 0;
    }
    cout << " > " << name << " perfomes a Unique Attack dealt " << dmg << "_dmg to " << target.name << endl;
    dmg = (dmg < 1) ? 1 : dmg;
    target.stats.hp -= dmg;
    if (!target.isAlive())
        cout << " >> " << target.name << " is dead !" << endl;
    return dmg;
}

void Unit::applyCC(Unit &target, int duration)
{
    cout << " > " << name << " stuns " << target.name << " for " << duration << " turns" << endl;
    target.crowdControl.is = true;
    target.crowdControl.duration = duration;
}

void Unit::applyDot(Unit &target, int duration, double scale)
{
    cout << " > " << name << " applies DOT dmg to " << target.name << " for " << duration << " turns" << endl;
    target.dots.emplace_back(Status(true, duration, scale));
}

double Unit::dotAttack(Unit &target, const Status &dot)
{
    double pen = (1 - sStats.penetration) < 0 ? 0 : (1 - sStats.penetration),
           dmgBn = (1 + sStats.dmgBonus),
           dmgRe = 1 - target.sStats.dmgReduction < 0 ? 0 : 1 - target.sStats.dmgReduction;
    double dmg = stats.atk * dot.scale - target.stats.def * pen;
    dmg = (dmg < 1) ? 1 : dmg;
    dmg = dmg * dmgBn * dmgRe;
    target.stats.hp -= dmg;
    cout << " > " << target.name << " receives " << dmg << " DOT dmg from " << name << endl;
    return dmg;
}

double Unit::trueAttack(Unit &target, double dmg)
{
    target.stats.hp -= dmg;
    target.stats.hp = target.stats.hp < 0 ? 0 : target.stats.hp;
    return dmg;
}

bool Unit::isCced()
{
    if (crowdControl.duration <= 0)
        crowdControl.is = false;
    crowdControl.duration--;
    return crowdControl.is;
}

bool Unit::isDotted(Unit &dotDmgDealer)
{
    for (Status &dot : dots)
    {
        if (dot.duration > 0)
            dotDmgDealer.dotAttack(*this, dot);
        else
            dot.is = false;
        dot.duration--;
    }
    return isAlive();
}

bool Unit::updateBadStatus(Unit &dotDmgDealer)
{
    if (!isDotted(dotDmgDealer))
    {
        cout << " > " << name << " is dead !" << endl;
        this_thread::sleep_for(chrono::milliseconds(750));
        return true;
    }

    if (isCced())
    {
        cout << " > " << name << " is unable to take action !" << endl;
        this_thread::sleep_for(chrono::milliseconds(750));
        return true;
    }
    return false;
}

void Unit::ultimate(Unit &target)
{
    cout << " > " << name << " activates *ULTIMATE*" << endl;
    double max_hp = stats.maxHp, max_atk = stats.maxAtk, max_def = stats.maxDef;
    double totalDmg = 0, amountHeal = 0;
    switch (type)
    {
    case 0:
        if (id == 0)
        {
            cout << "I am Atomic !";
            sStats.penetration += 0.6;
            totalDmg += attack(target, 1.8, 0);
            sStats.penetration -= 0.6;
        }
        else if (id == 1)
        {
            cout << "Kame kame haaaaaaaaaaa";
            critStats.rate += 0.95;
            totalDmg += attack(target, 2, 0);
            critStats.rate -= 0.95;
        }
        else if (id == 2)
        {
            cout << "Galtling gunnn";
            for (int i = 0; i < 5; i++)
                totalDmg += attack(target, 0.75, 0);
        }
        else if (id == 3)
        {
            cout << "Domain Expansion ! ";
            applyStatBuff(StatType::ATK, max_atk, 2, name);
            applySpecialBuff(SpecialStatType::DmgReduction, 0.3, 2, name);
        }
        else if (id == 4)
        {
            int dice1 = rng(1, 6), dice2 = rng(1, 6), dice3 = rng(1, 6);
            cout << "DICE 1 = " << dice1 << ", DICE 2 = " << dice2 << ", DICE 3 = " << dice3 << endl;
            double total = dice1 + dice2 + dice3;
            if (dice1 == dice2 == dice3)
                total *= 1.5;
            else if (dice1 == dice2 || dice1 == dice3 || dice2 == dice3)
                total *= 1.2;
            applyCritBuff(CritType::Rate, 0.45, 2, name);
            applyCritBuff(CritType::Dmg, total / 9.0, 2, name);
            applySpecialBuff(SpecialStatType::Penetration, total / 50.0, 2, name);
        }
        else if (id == 5)
        {
            cout << "Dragon transformation";
            applyStatBuff(StatType::ATK, stats.def * 5, 1, name);
            applySpecialBuff(SpecialStatType::DmgReduction, 0.3, 1, name);
            applyStatBuff(StatType::DEF, -stats.def, 1, name);
        }
        else if (id == 6)
        {
            cout << "Bloodlust activates";
            double hpLost = max_hp - stats.hp;
            heal(hpLost);
            applyStatBuff(StatType::ATK, (hpLost * 0.05 + max_hp * 0.15), 10, name);
            energy.regen = 0;
        }

        else if (id == 7)
        {
            cout << "Explosive arrow";
            totalDmg += attack(target, energy.current * 0.05, 0);
            energy.regen += 8;
        }
        else if (id == 8)
        {
            cout << "Speed of Light";
            applySpecialBuff(SpecialStatType::Evade, 0.5, 2, name);
            sStats.dmgBonus += 0.05;
        }
        else if (id == 9)
        {
            cout << "Your soul is mine";
            sStats.penetration += 1;
            double dmg = max_hp * 0.25;
            totalDmg = sAttack(target, dmg);
            sStats.penetration -= 1;
            heal(totalDmg * 0.2);
        }
        else if (id == 10)
        {
            cout << "Time to say bye. BOOM.";
            applyDot(target, 10, 0.55);
            sStats.penetration += 1;
            target.isDotted(*this);
            sStats.penetration -= 1;
        }
        else if (id == 11)
        {
            cout << "Dragon Flame";
            totalDmg += attack(target, 1.25, 0);
            applyDot(target, 3, 0.75);
        }

        break;
    case 1:
        if (id == 0)
        {
            cout << "Slime shoot !";
            totalDmg += attack(target, 1.7, 0);
        }
        else if (id == 1)
        {
            cout << "Goblin smashhh !";
            totalDmg += attack(target, 1.5, 0);
            if (totalDmg != 0)
                applyCC(target, 1);
        }
        else if (id == 2)
        {
            cout << "Golem strengthens !";
            applyStatBuff(StatType::ATK, stats.def * 0.4, 2, name);
            applyStatBuff(StatType::DEF, max_def * 0.8, 2, name);
        }
        else if (id == 3)
        {
            cout << "Skeleton focuses !";
            applyCritBuff(CritType::Rate, 0.95, 1, name);
            applyCritBuff(CritType::Dmg, 1.5, 1, name);
            applySpecialBuff(SpecialStatType::DmgReduction, 0.2, 1, name);
        }
        else if (id == 4)
        {
            cout << "Return from death";
            max_hp *= 1.25, max_atk *= 1.25, max_def *= 1.25;
            stats.hp = max_hp, stats.atk = max_atk, stats.def = max_def;
            energy.regen = 0;
        }
    default:
        break;
    }
    if (totalDmg > 0)
        cout << endl
             << " > [Total damage dealt: " << totalDmg << "]";
    cout << endl;
    energy.current = 0;
}

void Unit::applyStatBuff(StatType type, double value, int duration, string source, bool stackable)
{
    if (!stackable)
    {
        for (TimeModifier<StatType> &buff : statBuffs)
        {
            if (buff.type == type && buff.source == source)
            {
                if (value > buff.value)
                {
                    applyStatDelta(type, -buff.value);
                    buff.value = value;
                    applyStatDelta(type, value);
                }
                buff.remainingTurns = duration;
                return;
            }
        }
    }

    applyStatDelta(type, value);
    statBuffs.emplace_back(type, value, duration, source);
}

void Unit::applyCritBuff(CritType type, double value, int duration, string source, bool stackable)
{
    if (!stackable)
    {
        for (TimeModifier<CritType> &buff : critBuffs)
        {
            if (buff.type == type && buff.source == source)
            {
                if (value > buff.value)
                {
                    applyCritDelta(type, -buff.value);
                    buff.value = value;
                    applyCritDelta(type, value);
                }
                buff.remainingTurns = duration;

                return;
            }
        }
    }

    applyCritDelta(type, value);
    critBuffs.emplace_back(type, value, duration, source);
}

void Unit::applySpecialBuff(SpecialStatType type, double value, int duration, string source, bool stackable)
{
    if (!stackable)
    {
        for (TimeModifier<SpecialStatType> &buff : sStatBuffs)
        {
            if (buff.type == type && buff.source == source)
            {
                if (value > buff.value)
                {
                    applySpecialStatDelta(type, -buff.value);
                    buff.value = value;
                    applySpecialStatDelta(type, value);
                }
                buff.remainingTurns = duration;

                return;
            }
        }
    }

    applySpecialStatDelta(type, value);
    sStatBuffs.emplace_back(type, value, duration, source);
}

void Unit::updateBuffs()
{
    auto update = [](auto &buffs, auto applyDelta)
    {
        for (auto it = buffs.begin(); it != buffs.end();)
        {
            if (it->remainingTurns-- <= 0)
            {
                applyDelta(it->type, -it->value);
                it = buffs.erase(it);
            }
            else
                ++it;
        }
    };

    update(statBuffs, [this](StatType t, double d)
           { applyStatDelta(t, d); });
    update(critBuffs, [this](CritType t, double d)
           { applyCritDelta(t, d); });
    update(sStatBuffs, [this](SpecialStatType t, double d)
           { applySpecialStatDelta(t, d); });
}

void Unit::applyStatDelta(StatType type, double delta)
{
    switch (type)
    {
    case StatType::HP:
        stats.hp += delta;
        break;
    case StatType::ATK:
        stats.atk += delta;
        break;
    case StatType::DEF:
        stats.def += delta;
        break;
    }
}

void Unit::applyCritDelta(CritType type, double delta)
{
    switch (type)
    {
    case CritType::Rate:
        critStats.rate += delta;
        break;
    case CritType::Dmg:
        critStats.dmg += delta;
        break;
    }
}

void Unit::applySpecialStatDelta(SpecialStatType type, double delta)
{
    switch (type)
    {
    case SpecialStatType::Penetration:
        sStats.penetration += delta;
        break;
    case SpecialStatType::DmgBonus:
        sStats.dmgBonus += delta;
        break;
    case SpecialStatType::DmgReduction:
        sStats.dmgReduction += delta;
        break;
    case SpecialStatType::Evade:
        sStats.evade += delta;
    }
}

void Unit::setLevel(int lv)
{
    level = lv;
    update();
}

void Unit::applyOrb()
{
    stats.maxHp += stats.maxHp * orb.stats.maxHp;
    stats.maxAtk += stats.maxAtk * orb.stats.maxAtk;
    stats.maxDef += stats.maxDef * orb.stats.maxDef;
    critStats += orb.critStats;
    sStats += orb.sStats;
    if (id == orb.getId())
        stats *= 1.1;
}

void Unit::resetToBase()
{
    const double coeff = (type == 0) ? 1.05 : 1.08;
    stats.maxHp = stats.base_hp * pow(coeff, level);
    stats.maxAtk = stats.base_atk * pow(coeff, level);
    stats.maxDef = stats.base_def * pow(coeff, level);
}

void Unit::update()
{
    reCalStats();
    resetToBase();
    applyOrb();
    stats.reset();
}