#include "Configuration.hpp"

Configuration::Configuration(const string &fileName)
{
    dataFile = fileName;
    ifstream asset(fileName);
    ifstream save(saveFile);
    if (asset.is_open())
    {
        string line;
        while (getline(asset, line))
        {
            removeSpace(line);
            loadAsset(line);
        }
    }
    else
    {
        cout << "Assets can't be found";
        return;
    }
    if (save.is_open())
    {
        string line;
        while (getline(save, line))
        {
            removeSpace(line);
            loadSave(line);
        }
    }
    else
    {
        cout << "Saves can't be found";
    }
    setUnitLevel();
}

void Configuration::loadAsset(const string &str)
{
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
    char nameC[50];
    string toRead = type == "unit" ? "Unit=%49[^,],{%lf,%lf,%lf},%lf" : "Enemy=%49[^,],{%lf,%lf,%lf},%lf";
    double hp, atk, def, energy;
    int count = sscanf(str.c_str(), toRead.c_str(), nameC, &hp, &atk, &def, &energy);
    string name = nameC;
    int typeInNumber = (type == "unit") ? 0 : 1;
    Unit u(typeInNumber, name, {hp, atk, def}, energy);
    u.update();
    int size = (type == "unit") ? units.size() : enemies.size();
    if (size % 10 == 0 && size != 0)
        u.setRarity("UR");
    else if (size % 5 == 0 && size != 0)
        u.setRarity("SSR");
    if (type == "unit")
    {
        u.setID(size);
        units.emplace_back(u);
    }
    else if (type == "enemy")
    {
        u.setID(size);
        enemies.emplace_back(u);
    }
}

void Configuration::loadOrb(const string &str)
{
    string name;
    double hp = 0, atk = 0, def = 0, cr = 0, cd = 0, pen = 0, re = 0, bonus = 0, evade = 0;

    int start = str.find('=') + 1;
    int end = str.find(',', start);
    name = str.substr(start, end - start);

    start = str.find('{', end) + 1;
    end = str.find('}', start);
    string stats1 = str.substr(start, end - start);
    sscanf(stats1.c_str(), "%lf,%lf,%lf", &hp, &atk, &def);

    start = str.find('{', end) + 1;
    end = str.find('}', start);
    string stats2 = str.substr(start, end - start);
    sscanf(stats2.c_str(), "%lf,%lf", &cr, &cd);

    start = str.find('{', end) + 1;
    end = str.find('}', start);
    string stats3 = str.substr(start, end - start);
    sscanf(stats3.c_str(), "%lf,%lf,%lf,%lf", &pen, &re, &bonus, &evade);

    hp /= 100, atk /= 100, def /= 100, cr /= 100, cd /= 100, pen /= 100, re /= 100, bonus /= 100, evade /= 100;

    Orb orb(name, {hp, atk, def}, {cr, cd}, {pen, re, bonus, evade});
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
    string typeStr(name);
    guardians.emplace_back(Guardian(name, id, rPA));
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
    units = vector<Unit>();
    enemies = vector<Unit>();
    orbs = vector<Orb>();
    guardians = vector<Guardian>();
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