#include "Configuration.hpp"

Configuration::Configuration(const string &fileName)
{
    dataFile = fileName;
    ifstream asset(fileName);
    ifstream save(saveFile);
    if (!asset.is_open())
    {
        cout << "data.txt not found";
        return;
    }
    string line;
    while (getline(asset, line))
    {
        if (line.find("//") == 0)
        {
            cout << " - Loading " << line.substr(line.find("//") + 2, line.find("=") - line.find("//") - 2) << "..." << endl;
            continue;
        }
        loadAsset(line);
    }

    if (!save.is_open())
        cout << "save.txt not found";
    else
    {
        string line;
        while (getline(save, line))
        {
            removeSpace(line);
            loadSave(line);
        }
    }
    loadDes();
    setUnitLevel();
}

void Configuration::loadAsset(const string &str)
{
    if (str.find("Speed=") == 0)
        speed = stoi(str.substr(str.find("=") + 1));
    if (str.find("Unit=") == 0)
        loadUnit(str, "unit");
    else if (str.find("Enemy=") == 0)
        loadUnit(str, "enemy");
    else if (str.find("Orb=") == 0)
        loadOrb(str);
    else if (str.find("Guardian=") == 0)
        loadGuardian(str);
}

void Configuration::loadUnit(const string &str, string type)
{
    char name[50];
    string toRead = (type == "unit" ? "Unit=%49[^,],{%lf,%lf,%lf},%lf" : "Enemy=%49[^,],{%lf,%lf,%lf},%lf");
    double hp, atk, def, energy;
    int count = sscanf(str.c_str(), toRead.c_str(), name, &hp, &atk, &def, &energy);
    if (count != 5)
        throw runtime_error("Failed to parse input string: " + str);
    int typeInNumber = (type == "unit") ? 0 : 1;
    Unit u(typeInNumber, name, {hp, atk, def}, energy);
    int size = (type == "unit") ? units.size() : enemies.size();
    u.setID(size);
    if (type == "unit")
        units.emplace_back(u);
    else if (type == "enemy")
        enemies.emplace_back(u);
}

void Configuration::loadOrb(const string &str)
{
    char n[50];
    double hp, atk, def;
    double cr, cd;
    double dmgbonus, pen, ultbonus, dotbonus;
    double res_cc, res_dot, res_dmg;
    double hr_cc, hr_dot, hr_dmg;
    double evade, accuracy;

    int matched = sscanf(str.c_str(),
                         "Orb=%49[^,],BASE{%lf,%lf,%lf},CRIT{%lf,%lf},MOD{%lf,%lf,%lf,%lf},RES{%lf,%lf,%lf},HITRATE{%lf,%lf,%lf},AGI{%lf,%lf}",
                         n,
                         &hp, &atk, &def,
                         &cr, &cd,
                         &dmgbonus, &pen, &ultbonus, &dotbonus,
                         &res_dmg, &res_cc, &res_dot,
                         &hr_dmg, &hr_cc, &hr_dot,
                         &evade, &accuracy);
    Orb orb(n, BaseStats(hp / 100, atk / 100, def / 100), CritStats(cr / 100, cd / 100), Modifiers(dmgbonus / 100, pen / 100, ultbonus / 100, dotbonus / 100), Effect(res_dmg / 100, res_cc / 100, res_dot / 100), Effect(hr_dmg / 100, hr_cc / 100, hr_dot / 100), Agility(evade / 100, accuracy / 100));
    int size = orbs.size();
    orb.setID(size);
    if (size % 10 == 0 && size != 0)
        orb.setRarity("UR");
    else if (size % 5 == 0 && size != 0)
        orb.setRarity("SSR");
    orbs.emplace_back(orb);
}

void Configuration::loadGuardian(const string &str)
{
    char name[50];
    int rPA, id = guardians.size();
    sscanf(str.c_str(), "Guardian=%49[^,],%d", &name, &rPA);
    guardians.emplace_back(Guardian(name, id, rPA));
}

void Configuration::loadDes()
{
    const string path = "Data/description.txt";
    ifstream file(path);
    if (!file.is_open())
    {
        cout << "description.txt not found" << endl;
        return;
    }
    for (int i = 0; i < units.size(); i++)
    {
        string line;
        if (!getline(file, line))
            return;
        if (line.find("Unit=") == 0)
        {
            char des[500];
            sscanf(line.c_str(), "Unit={%499[^}]", &des);
            units[i].setDescription(des);
        }
    }
}

void Configuration::loadSave(const string &str)
{
    if (str.find("Currency=") == 0)
    {
        int start = str.find('=') + 1;
        int end = str.find(',', start);
        gold = std::stoi(str.substr(start, end - start));
        ruby = std::stoi(str.substr(end + 1));
    }
    else if (str.find("UnitOwned=") == 0)
    {
        int start = str.find('=') + 1;
        stringstream ss(str.substr(start));
        string val;
        int i = 0;
        while (getline(ss, val, ',') && i < units.size())
        {
            if (val == "1")
                units[i].setOwned();
            i++;
        }
    }
    else if (str.find("OrbOwned=") == 0)
    {
        int start = str.find('=') + 1;
        stringstream ss(str.substr(start));
        string val;
        int i = 0;
        while (getline(ss, val, ',') && i < orbs.size())
        {
            if (val == "1")
                orbs[i].setOwned();
            i++;
        }
    }
    else if (str.find("GuardianOwned=") == 0)
    {
        int start = str.find('=') + 1;
        stringstream ss(str.substr(start));
        string val;
        int i = 0;
        while (getline(ss, val, ',') && i < guardians.size())
        {
            if (val == "1")
                guardians[i].setOwned();
            i++;
        }
    }
    else if (str.find("Level=") == 0)
    {
        int start = str.find('=') + 1;
        int end = str.find(',', start);
        level = stoi(str.substr(start, end - start));
        currentExp = stoi(str.substr(end + 1));
    }
}

void Configuration::clearAll()
{
}

void Configuration::saveAll()
{
    ofstream save(saveFile);
    if (save.is_open())
    {
        save << "Level=" << level << "," << currentExp << endl;
        save << "Currency=" << gold << "," << ruby << endl;
        save << "UnitOwned=";
        for (int i = 0; i < units.size(); i++)
        {
            save << units[i].getOwned();
            if (i != units.size() - 1)
                save << ",";
        }
        save << endl
             << "OrbOwned=";
        for (int i = 0; i < orbs.size(); i++)
        {
            save << orbs[i].getOwned();
            if (i != orbs.size() - 1)
                save << ",";
        }
        save << endl
             << "GuardianOwned=";
        for (int i = 0; i < guardians.size(); i++)
        {
            save << guardians[i].getOwned();
            if (i != guardians.size() - 1)
                save << ",";
        }
    }
}

string Configuration::getDataPath() const
{
    return dataFile;
}

void Configuration::setUnitLevel()
{
    for (int i = 0; i < units.size(); i++)
    {
        units[i].setLevel(level);
    }
}

void Configuration::removeSpace(string &str)
{
    string temp;
    for (int i = 0; i < str.length(); i++)
    {
        if (isspace(str[i]))
            continue;
        temp += str[i];
    }
    str = temp;
}