#include "StatsCal.hpp"

void StatsCal::update(int level)
{
    coeff = 1;
    coeff *= pow(baseCoeff, level);
    setBase();
    mod.reset();
    crit.reset();
    baseBuffs.clear(), modBuffs.clear(), critBuffs.clear();
}

void StatsCal::setBase()
{
    base.maxHp = base.base_hp * coeff;
    base.maxAtk = base.base_atk * coeff;
    base.maxDef = base.base_def * coeff;
    base.reset();
}

double StatsCal::getHpLost()
{
    return abs(base.maxHp) - abs(base.hp);
}

double StatsCal::getFinalDmg(string scaleOn) const
{
    double dmg = (scaleOn == "ATK") ? base.atk : (scaleOn == "HP") ? base.maxHp
                                             : (scaleOn == "DEF")  ? base.def
                                                                   : 0;
    dmg = dmg * (1 + mod.dmgBonus) * ((isCrit()) ? (1 + crit.dmg) : 1);
    return dmg;
}

double StatsCal::getFinalDef(const StatsCal &other, double K) const
{
    double pen = max(0.0, 1.0 - other.mod.penetration);
    double def = base.def * pen;
    double denominator = def + K * coeff;

    if (denominator <= 0.0)
        return 0.0;

    double reduceDmg = (1.0 - def / denominator) * (1.0 - mod.dmgReduction);
    return reduceDmg;
}

bool StatsCal::isCrit() const
{
    double critHit = (crit.rate > 1) ? 1 : crit.rate;
    return rngRate(1 - critHit, critHit) == 1;
}

void StatsCal::buffBase(BaseType type, double value, int duration, string source, bool stackable)
{
    if (!stackable)
    {
        for (TimeModifier<BaseType> &buff : baseBuffs)
        {
            if (buff.type == type && buff.source == source)
            {
                if (value > buff.value)
                {
                    addBaseBuff(type, -buff.value);
                    buff.value = value;
                    addBaseBuff(type, value);
                }
                buff.remainingTurns = duration;
                return;
            }
        }
    }

    addBaseBuff(type, value);
    baseBuffs.emplace_back(type, value, duration, source);
}

void StatsCal::buffMod(ModType type, double value, int duration, string source, bool stackable)
{
    if (!stackable)
    {
        for (TimeModifier<ModType> &buff : modBuffs)
        {
            if (buff.type == type && buff.source == source)
            {
                if (value > buff.value)
                {
                    addModBuff(type, -buff.value);
                    buff.value = value;
                    addModBuff(type, value);
                }
                buff.remainingTurns = duration;
                return;
            }
        }
    }

    addModBuff(type, value);
    modBuffs.emplace_back(type, value, duration, source);
}

void StatsCal::buffCrit(CritType type, double value, int duration, string source, bool stackable)
{
    if (!stackable)
    {
        for (TimeModifier<CritType> &buff : critBuffs)
        {
            if (buff.type == type && buff.source == source)
            {
                if (value > buff.value)
                {
                    addCritBuff(type, -buff.value);
                    buff.value = value;
                    addCritBuff(type, value);
                }
                buff.remainingTurns = duration;
                return;
            }
        }
    }

    addCritBuff(type, value);
    critBuffs.emplace_back(type, value, duration, source);
}

void StatsCal::addBaseBuff(BaseType type, double value)
{
    switch (type)
    {
    case BaseType::HP:
    {
        double currentHpByPercentage = base.hp / base.maxHp;
        base.maxHp += value;
        base.hp = base.maxHp * currentHpByPercentage;
        break;
    }
    case BaseType::ATK:
        base.atk += value;
        break;
    case BaseType::DEF:
        base.def += value;
        break;
    default:
        break;
    }
}
void StatsCal::addModBuff(ModType type, double value)
{
    switch (type)
    {
    case ModType::DMGBONUS:
    {
        mod.dmgBonus += value;
        break;
    }
    case ModType::DMGREDUCTION:
        mod.dmgReduction += value;
        break;
    case ModType::PENETRATION:
        mod.penetration += value;
        break;
    case ModType::EVADE:
        mod.evade += value;
        break;
    default:
        break;
    }
}

void StatsCal::addCritBuff(CritType type, double value)
{
    switch (type)
    {
    case CritType::RATE:
        crit.rate += value;
        break;
    case CritType::DMG:
        crit.dmg += value;
        break;
    default:
        break;
    }
}

void StatsCal::updateBuffs()
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

    update(baseBuffs, [this](BaseType t, double d)
           { addBaseBuff(t, d); });
    update(critBuffs, [this](CritType t, double d)
           { addCritBuff(t, d); });
    update(modBuffs, [this](ModType t, double d)
           { addModBuff(t, d); });
}
