#include "Unit.hpp"
#include "Functions.hpp"

Unit::Unit(int t, string n, BaseStats b, Energy e) : type(t), name(n), energy(e), level(0), stats(t, b)
{
    orb = Orb();
    update();
}

string Unit::info()
{
    stringstream ss;
    ss << " " << string(2, '*') << name << " (Energy Cap: " << energy.max << " - Owned: " << (owned ? "Yes" : "No");
    ss << ")" << string(2, '*') << endl;
    ss << stats.base.info();
    ss << stats.crit.info();
    ss << stats.mod.info();
    if (!stats.resistance.info().empty())
        ss << " - Resistance -> " << stats.resistance.info();
    if (!stats.hitRate.info().empty())
        ss << " - Hit Rate ->" << stats.hitRate.info();
    ss << stats.agility.info();
    if (!description.empty())
        ss << " + " << description << '.' << endl;
    return ss.str();
}

void Unit::displayStats()
{
    cout << " <" << name << "> - Energy: (" << energy.current << "/" << energy.max << ")" << endl;
    cout << stats.base.info();
    cout << stats.crit.info();
    cout << stats.mod.info();
    if (!stats.resistance.info().empty())
        cout << " - Resistance = " << stats.resistance.info();
    if (!stats.hitRate.info().empty())
        cout << " - Hit Rate = " << stats.hitRate.info();
    cout << stats.agility.info();
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

void Unit::setDescription(string des)
{
    description = des;
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
    if (stats.base.hp < 0)
        stats.base.hp = 0;
    return stats.base.hp > 0;
}

bool Unit::isEnoughEnergy()
{
    return energy.current >= energy.max;
}

bool Unit::isEvaded(Unit &other)
{
    if (stats.isEvade(other.stats))
    {
        cout << " > " << name << " evaded the attack" << endl;
        energy.current += energy.regen * 0.2;
        other.energy.current += other.energy.regen * 0.4;
        return true;
    }
    return false;
}

void Unit::setID(int i)
{
    id = i;
}

void Unit::heal(double amount)
{
    cout << " > " << name << " heals " << amount << "_hp" << endl;
    stats.base.hp += amount;
    stats.base.hp = (stats.base.hp > stats.base.maxHp) ? stats.base.maxHp : stats.base.hp;
}

double Unit::dmgCal(const Unit &target, double scale, StatsCal::BaseType scaleOn)
{
    double finalDmg = stats.getFinalDmg(scaleOn) * scale * target.stats.getFinalDef(this->stats);
    return finalDmg;
}

double Unit::attack(Unit &target, double scale, bool isUltimate, StatsCal::BaseType scaleOn)
{
    if (target.isEvaded(*this))
        return 0;
    int energyRegen = (isUltimate) ? 0 : 1;
    double ultBonus = (isUltimate ? (1 + stats.mod.ultDmgBonus) : 1);
    double dmg = dmgCal(target, scale, scaleOn) * getLimit(ultBonus, 1, ultBonus);
    cout << " > " << name << " dealt " << dmg << "_dmg to " << target.name << endl;
    target.stats.base.hp -= dmg;
    energy.current += energy.regen * energyRegen;
    target.energy.current += target.energy.regen * energyRegen / 5;
    if (!target.isAlive())
        cout << " >> " << target.name << " is dead !" << endl;
    return dmg;
}

void Unit::applyCC(Unit &target, int duration)
{
    if (!stats.effectHit(target.stats, StatsCal::EffectType::CC))
    {
        cout << " > " << target.name << " resists Crowd Control" << endl;
        return;
    }
    cout << " > " << name << " stuns " << target.name << " for " << duration << " turns" << endl;
    target.crowdControl = Status(true, duration);
}

void Unit::applyDot(Unit &target, int duration, double scale)
{
    if (!stats.effectHit(target.stats, StatsCal::EffectType::DOT))
    {
        cout << " > " << target.name << " resists DOT" << endl;
        return;
    }
    cout << " > " << name << " applies DOT dmg to " << target.name << " for " << duration << " turns" << endl;
    target.dots.emplace_back(Status(true, duration, scale));
}

void Unit::applyCounter(int duration, double scale)
{
    cout << " > " << name << " enables Counter Attack Mode" << endl;
    counterAtk = Status(true, duration, scale);
}

double Unit::dotAttack(Unit &target, const Status &dot)
{
    double dmg = stats.getFinalDmg(StatsCal::BaseType::ATK, false) * dot.scale * (1 + stats.mod.dotDmgBonus) * target.stats.getFinalDef(this->stats);
    target.stats.base.hp -= dmg;
    cout << " > " << target.name << " receives " << dmg << "_DOT dmg from " << name << endl;
    return dmg;
}

double Unit::trueAttack(Unit &target, double scale, StatsCal::BaseType scaleOn)
{
    stats.mod.penetration += 100, stats.mod.dmgBonus -= 100, stats.mod.ultDmgBonus -= 100, stats.hitRate.dmg += 100, stats.crit.rate -= 100;
    cout << endl
         << name << " deals true damage to " << target.name << endl;
    double finalDmg = attack(target, scale, true, scaleOn);
    stats.mod.penetration -= 100, stats.mod.dmgBonus += 100, stats.mod.ultDmgBonus += 100, stats.hitRate.dmg -= 100, stats.crit.rate += 100;
    return finalDmg;
}

double Unit::counterAttack(Unit &target, double scale, StatsCal::BaseType scaleOn)
{
    if (!isAlive() || target.isEvaded(*this))
        return 0;
    double dmg = dmgCal(target, scale, scaleOn);
    cout << " > " << name << " strikes back, dealing " << dmg << "_dmg to " << target.name << endl;
    target.stats.base.hp -= dmg;
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

double Unit::isCounter(Unit &target)
{
    double dmg = 0;
    if (counterAtk.duration > 0 && !crowdControl.is)
        dmg+=counterAttack(target, counterAtk.scale);
    else
        counterAtk.is = false;
    counterAtk.duration--;
    return dmg;
}

bool Unit::updatePreStatus(Unit &dotDmgDealer)
{
    isDotted(dotDmgDealer);
    if (isCced())
    {
        cout << " > " << name << " is unable to take action !" << endl;
        return true;
    }
    return false;
}

double Unit::ultimate(Unit &target)
{
    cout << " > " << name << " activates *ULTIMATE*" << endl;
    double max_hp = stats.base.maxHp, max_atk = stats.base.maxAtk, max_def = stats.base.maxDef;
    double totalDmg = 0, amountHeal = 0;
    switch (type)
    {
    case 0:
        if (id == 0)
        {
            cout << "I am Atomic !";
            stats.mod.penetration += 0.4;
            totalDmg += attack(target, 2, true);
            stats.mod.penetration -= 0.4;
        }
        else if (id == 1)
        {
            cout << "Kame kame haaaaaaaaaaa";
            stats.crit.rate += 0.95;
            totalDmg += attack(target, 2.6, true);
            stats.crit.rate -= 0.95;
        }
        else if (id == 2)
        {
            cout << "Galtling gunnn";
            for (int i = 0; i < 4; i++)
                totalDmg += attack(target, 0.8, true);
        }
        else if (id == 3)
        {
            cout << "Domain Expansion ! ";
            stats.buffBase(StatsCal::BaseType::ATK, max_atk, 2, name);
            stats.buffMod(StatsCal::ModType::DMGBONUS, 0.4, 2, name);
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
            stats.buffCrit(StatsCal::CritType::RATE, 0.5, 2, name);
            stats.buffCrit(StatsCal::CritType::DMG, total / 9.0, 2, name);
            stats.buffMod(StatsCal::ModType::PENETRATION, total / 50.0, 2, name);
        }
        else if (id == 5)
        {
            cout << "Dragon transformation";
            stats.buffBase(StatsCal::BaseType::ATK, stats.base.def * 5, 1, name);
            stats.buffEffect(StatsCal::EffectType::CC, 0.4, 2, name, true);
        }
        else if (id == 6)
        {
            cout << "Bloodlust activates";
            double lost = stats.getHpLost();
            heal(lost);
            stats.base.atk += (lost * 0.1 + max_hp * 0.1);
            energy.max = 999999;
        }

        else if (id == 7)
        {
            cout << "Explosive arrow";
            totalDmg += attack(target, energy.current * 0.05, true);
            energy.regen += 11;
        }
        else if (id == 8)
        {
            cout << "Speed of Light";
            stats.buffAgility(StatsCal::AgilityType::EVADE, 0.4, 2, name);
            stats.agility.evade += 0.01;
            stats.mod.dmgBonus += 0.05;
        }
        else if (id == 9)
        {
            cout << "Your soul is mine";
            totalDmg = attack(target, 0.4, true, StatsCal::BaseType::HP);
            heal(totalDmg * 0.2);
        }
        else if (id == 10)
        {
            cout << "Time to say bye\n  BOOM.";
            stats.buffEffect(StatsCal::EffectType::DOT, 1.2, 99, name);
            applyDot(target, 10, 0.25);
            stats.mod.dmgBonus += 0.6;
            target.isDotted(*this);
            stats.mod.dmgBonus -= 0.6;
        }
        else if (id == 11)
        {
            cout << "Dragon Flame";
            stats.buffEffect(StatsCal::EffectType::DOT, 1, 99, name);
            totalDmg += attack(target, 1.5, true);
            applyDot(target, 3, 0.8);
        }
        else if (id == 12)
        {
            cout << "Excaliburr !";
            attack(target, 8, true);
        }
        else if (id == 13)
        {
            cout << "Cat's Scratch ";
            double enemyCurrentHp = target.stats.base.hp / target.stats.base.maxHp;
            stats.buffMod(StatsCal::ModType::ULTDMGBONUS, enemyCurrentHp * 3, 1, name);
            for (int i = 0; i < 5; i++)
                totalDmg += attack(target, 0.6, true);
        }
        else if (id == 14)
        {
            cout << "Time malnipulation";
            stats.agility.accuracy += 100;
            totalDmg += trueAttack(target, 2.5);
            stats.agility.accuracy -= 100;
            stats.buffEffect(StatsCal::EffectType::DMG, 1, 1, name, true);
        }
        else if (id == 15)
        {
            cout << "Self strengthens";
            applyCounter(2, 1);
            stats.buffEffect(StatsCal::EffectType::DMG, 0.2, 2, name, true);
        }

        break;
    case 1:
        if (id == 0)
        {
            cout << "Slime shoot !";
            totalDmg += attack(target, 1.8, true);
        }
        else if (id == 1)
        {
            cout << "Goblin smashhh !";
            totalDmg += attack(target, 1.5, true);
            if (totalDmg != 0)
                applyCC(target, 1);
        }
        else if (id == 2)
        {
            cout << "Golem strengthens !";
            stats.buffBase(StatsCal::BaseType::ATK, stats.base.def * 0.5, 2, name);
            stats.buffBase(StatsCal::BaseType::DEF, max_def * 1, 2, name);
        }
        else if (id == 3)
        {
            cout << "Skeleton focuses !";
            stats.buffCrit(StatsCal::CritType::RATE, 0.95, 1, name);
            stats.buffCrit(StatsCal::CritType::DMG, 1.5, 1, name);
            stats.buffEffect(StatsCal::EffectType::DMG, 0.2, 1, name, true);
        }
        else if (id == 4)
        {
            cout << "Return from death";
            stats.base.maxHp *= 1.25, stats.base.maxAtk *= 1.25, stats.base.maxDef *= 1.25;
            heal(stats.base.maxHp - stats.base.hp), stats.base.atk = stats.base.maxAtk, stats.base.def = stats.base.maxDef;
            energy.regen = 0;
        }
        else if (id == 5)
        {
            cout << "I'm dead";
            stats.buffBase(StatsCal::BaseType::ATK, stats.base.def, 5, name);
            stats.buffBase(StatsCal::BaseType::DEF, -stats.base.def, 5, name);
            stats.buffEffect(StatsCal::EffectType::DMG, 1, 1, name, true);
        }
        else if (id == 6)
        {
            cout << "I am invisible";
            stats.base.atk *= 2;
            stats.base.def *= 2;
            stats.buffAgility(StatsCal::AgilityType::EVADE, 0.6, 99, name);
        }
        else if (id == 666)
        {
            cout << "Apocalypse soon comes";
            stats.base.atk = stats.base.atk * stats.base.atk;
        }
    default:
        break;
    }
    if (totalDmg > 0)
        cout << " > [Total damage dealt: " << totalDmg << "]";
    cout << endl;
    energy.current = 0;
    return totalDmg;
}

void Unit::setLevel(int lv)
{
    level = lv;
    update();
}

void Unit::applyOrb()
{
    BaseStats b = BaseStats(stats.base.maxHp * orb.base.hp, stats.base.maxAtk * orb.base.atk, stats.base.maxDef * orb.base.def);
    stats.base += b;
    stats.crit += orb.crit;
    stats.mod += orb.mod;
    stats.resistance += orb.resistance;
    stats.hitRate += orb.hitRate;
    stats.agility += orb.agility;
    if (id == orb.getId())
        stats.base *= 1.1;
}

void Unit::update()
{
    stats.update(level);
    applyOrb();
    stats.base.reset();
}