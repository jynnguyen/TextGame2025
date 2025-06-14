#pragma once
#include "main.hpp"
#include "Components.hpp"
#include "Functions.hpp"

template <typename T>
class TimeModifier
{
public:
    T type;
    double value;
    int remainingTurns;
    string source;

    TimeModifier(T t, double v, int duration, string source)
        : type(t), value(v), remainingTurns(duration), source(source) {}
};

class StatsCal
{
public:
    StatsCal(int unitType, BaseStats b = {1, 1, 1}, CritStats cr = {0.05, 0.5});

    enum class BaseType
    {
        HP,
        ATK,
        DEF
    };
    enum class CritType
    {
        RATE,
        DMG
    };
    enum class ModType
    {
        DMGBONUS,
        PENETRATION,
        ULTDMGBONUS,
        DOTDMGBONUS
    };
    enum class EffectType
    {
        CC,
        DOT,
        DMG
    };
    enum class AgilityType
    {
        EVADE,
        ACCURACY
    };
    void setBase();
    void buffBase(BaseType type, double value, int duration = 99, string source = "", bool stackable = false);
    void buffMod(ModType type, double value, int duration = 99, string source = "", bool stackable = false);
    void buffCrit(CritType type, double value, int duration = 99, string source = "", bool stackable = false);
    void buffEffect(EffectType type, double value, int duration = 99, string source = "", bool isResistance = false, bool stackable = false);
    void buffAgility(AgilityType type, double value, int duration = 99, string source = "", bool stackable = false);
    void updateBuffs();
    void update(int level);

    double getHpLost();
    bool effectHit(StatsCal& other, EffectType type);
    bool isEvade(StatsCal& other);
    double getFinalDmg(BaseType scaleOn = BaseType::ATK, bool canCrit = true) const;
    double getFinalDef(const StatsCal& other, double K = 50) const;

    double coeff, baseCoeff;
    BaseStats base;
    Modifiers mod;
    CritStats crit;
    Effect resistance;
    Effect hitRate;
    Agility agility;

private:
    vector<TimeModifier<BaseType>> baseBuffs;
    vector<TimeModifier<ModType>> modBuffs;
    vector<TimeModifier<CritType>> critBuffs;
    vector<TimeModifier<EffectType>> resBuffs;
    vector<TimeModifier<EffectType>> ehrBuffs;
    vector<TimeModifier<AgilityType>> agiBuffs;

    bool isCrit() const;
    void addBaseBuff(BaseType type, double value);
    void addModBuff(ModType type, double value);
    void addCritBuff(CritType type, double value);
    void addEResBuff(EffectType type, double value);
    void addEhrBuff(EffectType type, double value);
    void addAgiBuff(AgilityType type, double value);
};
