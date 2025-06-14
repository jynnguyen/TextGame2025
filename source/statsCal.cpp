#include "StatsCal.hpp"

StatsCal::StatsCal(int unitType, int level, BaseStats b, CritStats cr) : base(b), crit(cr)
{
    resistance = Effect();
    hitRate = Effect();
    agility = Agility();
    baseCoeff = (unitType == 0) ? 1.05 : 1.08;
    update(level);
}

void StatsCal::update(int level)
{
    coeff = 1;
    coeff *= pow(baseCoeff, level);
    setBase();
    mod.reset(), crit.reset(), resistance.reset(), hitRate.reset(), agility.reset();
    baseBuffs.clear(), modBuffs.clear(), critBuffs.clear(), resBuffs.clear(), ehrBuffs.clear(), agiBuffs.clear();
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

bool StatsCal::effectHit(StatsCal &target, EffectType type)
{
    if (type == EffectType::CC)
    {
        double hit = getLimit(1 + hitRate.cc - target.resistance.cc, 0, 1);
        return rngRate(1 - hit, hit) == 1;
    }
    else if (type == EffectType::DOT)
    {
        double hit = getLimit(hitRate.dot - resistance.dot, 0, 1);
        return rngRate(1 - hit, hit) == 1;
    }
    else
        return false;
}

double StatsCal::getFinalDmg(BaseType scaleOn, bool canCrit) const
{
    double dmg = (scaleOn == BaseType::ATK) ? base.atk : (scaleOn == BaseType::HP) ? base.maxHp
                                                     : (scaleOn == BaseType::DEF)  ? base.def
                                                                                   : 0;
    double bonus = 1 + mod.dmgBonus;
    dmg = dmg * getLimit(bonus, 1.0, bonus);
    if (canCrit && isCrit())
        dmg *= (1 + crit.dmg);
    return dmg;
}

double StatsCal::getFinalDef(const StatsCal &other, double K) const
{
    double pen = 1.0 - other.mod.penetration;
    double def = base.def * getLimit(pen, 0.0, pen);
    double denominator = def + K * coeff;
    double dmgReduction = (1.0 - resistance.dmg + other.hitRate.dmg);

    if (denominator <= 0.0)
        return 0.0;

    double reduceDmg = (1.0 - def / denominator) * getLimit(dmgReduction, 0, 1);
    return reduceDmg;
}

bool StatsCal::isCrit() const
{
    double critHit = getLimit(crit.rate,0,1);
    bool result = rngRate(1.0 - critHit, critHit) == 1;
    if (result)
        cout << " Critical Hit !" << endl;
    return result;
}

bool StatsCal::isEvade(StatsCal &other)
{
    double evade = getLimit(agility.evade - other.agility.accuracy,0,1);
    return rngRate(1.0 - evade, evade) == 1;
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

void StatsCal::buffEffect(EffectType type, double value, int duration, string source, bool isResistance, bool stackable)
{
    if (!stackable)
    {
        if (isResistance)
        {
            for (TimeModifier<EffectType> &buff : resBuffs)
            {
                if (buff.type == type && buff.source == source)
                {
                    if (value > buff.value)
                    {
                        addEResBuff(type, -buff.value);
                        buff.value = value;
                        addEResBuff(type, buff.value);
                    }
                    buff.remainingTurns = duration;
                    return;
                }
            }
        }
        else
        {
            for (TimeModifier<EffectType> &buff : ehrBuffs)
            {
                if (buff.type == type && buff.source == source)
                {
                    if (value > buff.value)
                    {
                        addEhrBuff(type, -buff.value);
                        buff.value = value;
                        addEhrBuff(type, buff.value);
                    }
                    buff.remainingTurns = duration;
                    return;
                }
            }
        }
    }
    if (isResistance)
    {
        addEResBuff(type, value);
        resBuffs.emplace_back(type, value, duration, source);
    }
    else
    {
        addEhrBuff(type, value);
        ehrBuffs.emplace_back(type, value, duration, source);
    }
}

void StatsCal::buffAgility(AgilityType type, double value, int duration, string source, bool stackable)
{
    if (!stackable)
    {
        for (TimeModifier<AgilityType> &buff : agiBuffs)
        {
            if (buff.type == type && buff.source == source)
            {
                if (value > buff.value)
                {
                    addAgiBuff(type, -buff.value);
                    buff.value = value;
                    addAgiBuff(type, buff.value);
                }
                buff.remainingTurns = duration;
                return;
            }
        }
    }

    addAgiBuff(type, value);
    agiBuffs.emplace_back(type, value, duration, source);
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
    case ModType::ULTDMGBONUS:
        mod.ultDmgBonus += value;
        break;
    case ModType::PENETRATION:
        mod.penetration += value;
        break;
    case ModType::DOTDMGBONUS:
        mod.dotDmgBonus += value;
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
            {
                ++it;
            }
        }
    };

    update(baseBuffs, [this](BaseType t, double d)
           { addBaseBuff(t, d); });
    update(critBuffs, [this](CritType t, double d)
           { addCritBuff(t, d); });
    update(modBuffs, [this](ModType t, double d)
           { addModBuff(t, d); });
    update(resBuffs, [this](EffectType t, double d)
           { addEResBuff(t, d); });
    update(ehrBuffs, [this](EffectType t, double d)
           { addEhrBuff(t, d); });
    update(agiBuffs, [this](AgilityType t, double d)
           { addAgiBuff(t, d); });
}

void StatsCal::addEResBuff(EffectType type, double value)
{
    switch (type)
    {
    case EffectType::CC:
        resistance.cc += value;
        break;
    case EffectType::DMG:
        resistance.dmg += value;
        break;
    case EffectType::DOT:
        resistance.dot += value;
        break;
    default:
        break;
    }
}

void StatsCal::addEhrBuff(EffectType type, double value)
{
    switch (type)
    {
    case EffectType::CC:
        hitRate.cc += value;
        break;
    case EffectType::DMG:
        hitRate.dmg += value;
        break;
    case EffectType::DOT:
        hitRate.dot += value;
        break;
    }
}

void StatsCal::addAgiBuff(AgilityType type, double value)
{
    switch (type)
    {
    case AgilityType::EVADE:
        agility.evade += value;
        break;
    case AgilityType::ACCURACY:
        agility.accuracy += value;
        break;
    }
}