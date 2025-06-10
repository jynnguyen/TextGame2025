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
    StatsCal(int unitType, int level = 0, BaseStats b = {1,1,1} , CritStats cr ={0,0}, Modifiers m={0,0,0,0}) : base(b), crit(cr), mod(m)
    {
        baseCoeff = (unitType == 0)?1.05:1.08;
        update(level);
    }
    enum class ModType
    {
        PENETRATION,
        DMGBONUS,
        DMGREDUCTION,
        EVADE
    };
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
    void setBase();
    void buffBase(BaseType type, double value, int duration = 99, string source = "", bool stackable = false);
    void buffMod(ModType type, double value, int duration = 99, string source = "", bool stackable = false);
    void buffCrit(CritType type, double value, int duration = 99, string source = "", bool stackable = false);
    void updateBuffs();
    void update(int level);
    double getHpLost();
    double getFinalDmg(string scaleOn = "ATK") const;
    double getFinalDef(const StatsCal &other, double K = 100) const;

    double coeff, baseCoeff;
    BaseStats base;
    Modifiers mod;
    CritStats crit;
    vector<TimeModifier<BaseType>> baseBuffs;
    vector<TimeModifier<ModType>> modBuffs;
    vector<TimeModifier<CritType>> critBuffs;

    bool isCrit() const;
    void addBaseBuff(BaseType type, double value);
    void addModBuff(ModType type, double value);
    void addCritBuff(CritType type,double value);
};
