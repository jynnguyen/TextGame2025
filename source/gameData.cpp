#include "GameData.hpp"

GameData::GameData(const string &fileName) : config(fileName)
{
    gold = config.gold, ruby = config.ruby;
    level.set(config.level);
    level.setCurrentExp(config.currentExp);
    for (const Unit &u : config.units)
        units.emplace_back(u);
    for (const Unit &e : config.enemies)
        enemies.emplace_back(e);
    for (const Orb &o : config.orbs)
        orbs.emplace_back(o);
    for (const Guardian &g : config.guardians)
        guardians.emplace_back(g);
    config.clearAll();
}

Unit GameData::spawnEnemy()
{
    return enemies[rng(0, enemies.size() - 1)];
}

void GameData::save()
{
    config.units = units;
    config.orbs = orbs;
    config.guardians = guardians;
    config.gold = gold;
    config.ruby = ruby;
    config.level = level.get();
    config.currentExp = level.getCurrentExp();
    config.saveAll();
}

void GameData::inventory(string type)
{
    if (type == "unit" || type == "u")
    {
        printInventory<Unit>(
            units, "UNIT",
            [](const Unit &u)
            { return u.getId(); },
            [](const Unit &u)
            { return u.getName(); },
            [](const Unit &u)
            { return u.getOwned(); });
    }
    else if (type == "orb" || type == "o")
    {
        printInventory<Orb>(
            orbs, "ORB",
            [](const Orb &o)
            { return o.getId(); },
            [](const Orb &o)
            { return o.getName(); },
            [](const Orb &o)
            { return o.getOwned(); });
    }
    else if (type == "guardian" || "g")
    {
        printInventory<Guardian>(
            guardians, "GUARDIAN",
            [](const Guardian &g)
            { return g.getId(); },
            [](const Guardian &g)
            { return g.getName(); },
            [](const Guardian &g)
            { return g.getOwned(); });
    }
    else
    {
        cout << " > Invalid inventory " << endl;
    }
}

void GameData::setUnitLevel()
{
    for (Unit &u : units)
        u.setLevel(level.get());
}