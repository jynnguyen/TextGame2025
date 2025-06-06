#include "Game.hpp"
#include "Functions.hpp"

// GAME INIT
Game::Game(const string &fileName) : gameData(fileName)
{
}

void Game::run()
{
    while (gameRunning)
    {
        menu();
        gameData.save();
    }
}

// GAME SCENES
void Game::menu()
{
    cout << endl
         << string(50, '*') << endl;
    cout << "  ==== MAIN MENU ====  " << endl;
    cout << " > Command: ";
    cin >> playerInput;
    handleInput();
}

void Game::handleInput()
{
    if (playerInput == "help" || playerInput == "?")
    {
        cout << "" << endl;
    }
    else if (playerInput == "quit")
    {
        cout << " > You quit the game" << endl;
        gameRunning = false;
    }
    else if (playerInput == "id")
        displayInfo();
    else if (playerInput == "info" || playerInput == "lv" || playerInput == "level")
        gameData.level.info();
    else if (playerInput == "deploy" || playerInput == "de")
        deployUnit();
    else if (playerInput == "select" || playerInput == "sel")
        selectGuardian();
    else if (playerInput == "equip" || playerInput == "eq")
        equipOrb();
    else if (playerInput == "inventory" || playerInput == "inv")
        inventory();
    else if (playerInput == "setspeed" || playerInput == "ss")
        setRaidSpeed();
    else if (playerInput == "raid")
        raid();
    else if (playerInput == "gacha" || playerInput == "ga")
        gacha();
    else if (playerInput == "levelup" || playerInput == "lvup")
        levelUp();
    else if (playerInput == "admin")
        adminMode();
    else if (playerInput == "reload" || playerInput == "re")
        reloadData();
    else
        cout << " > Invalid command type 'help' to get valid commands" << endl;
    playerInput.clear();
}

void Game::raid()
{
    if (unitIdx < 0 || unitIdx >= gameData.units.size())
    {
        cout << " > Deploy unit first" << endl;
        return;
    }
    Unit u = gameData.units[unitIdx];
    Unit e = gameData.spawnEnemy();
    Guardian g = (gIdx >= 0 && gIdx < gameData.guardians.size()) ? gameData.guardians[gIdx] : Guardian();
    Raid r(u, e, g, raidSpeed);
    if (r.start())
    {
        cout << " > You won !" << endl;
        int extraExp = rng(1, 3), extraGold = rng(10, 20), extraRuby = 1;
        gameData.level.setCurrentExp(gameData.level.getCurrentExp() + extraExp);
        gameData.gold += extraGold, gameData.ruby += extraRuby;
        cout << " >> You gain " << extraExp << "_exp | " << extraGold << "_gold | " << extraRuby << "_ruby" << endl;
    }
}

// Game Features
void Game::deployUnit()
{
    string name = (unitIdx >= 0 && unitIdx < gameData.units.size()) ? gameData.units[unitIdx].getName() : "";
    cout << "  == UNIT DEPLOYMENT ==   " << endl;
    cout << "Current unit deployment: " << name << endl;
    string input;
    cout << "Select unit to deploy: ";
    cin >> input;
    cout << endl
         << " > ";
    for (int i = 0; i < input.length(); i++)
    {
        if (!isdigit(input[i]))
        {
            cout << "Invalid input" << endl;
            return;
        }
    }
    int id = stoi(input);
    if ((id) < 0 || (id) >= gameData.units.size())
    {
        cout << "Unit selected is out of range" << endl;
        return;
    }
    else if (!gameData.units[id].getOwned())
    {
        cout << "You don't own this Unit";
        return;
    }
    unitIdx = id;
    cout << "Deploy " << gameData.units[unitIdx].getName() << " successfully" << endl;
    cout << endl;
}

void Game::selectGuardian()
{
    string name = (gIdx >= 0 && gIdx < gameData.guardians.size()) ? gameData.guardians[gIdx].getName() : "";
    cout << "  >[GUARDIAN DEPLOYMENT]<  " << endl;
    cout << "Current guardian deployment: " << name << endl;
    string input;
    cout << "Select guardian to deploy: ";
    cin >> input;
    cout << endl
         << " > ";
    for (int i = 0; i < input.length(); i++)
    {
        if (!isdigit(input[i]))
        {
            cout << "Invalid input" << endl;
            return;
        }
    }
    int id = stoi(input);
    if ((id) < 0 || (id) >= gameData.guardians.size())
    {
        cout << "Guardian selected is out of range" << endl;
        return;
    }
    else if (!gameData.guardians[id].getOwned())
    {
        cout << "You don't own this Guardian";
        return;
    }
    gIdx = id;
    cout << "Deploy " << gameData.guardians[gIdx].getName() << " successfully" << endl;
    cout << endl;
}

void Game::equipOrb()
{
    string u, o;
    cout << "Select Unit - Orb to equip: ";
    cin >> u;
    if (u != "auto")
        cin >> o;
    cout << endl
         << " > ";
    if (u == "auto" || o == "auto")
    {
        cout << "Successfully equip best for all units" << endl;
        for (int i = 0; i < gameData.units.size() && i < gameData.orbs.size(); i++)
        {
            if (!gameData.units[i].getOwned() || !gameData.orbs[i].owned)
                continue;
            gameData.units[i].orb = gameData.orbs[i];
            gameData.units[i].update();
            cout << " > Successfully equip [" << gameData.units[i].getName() << "] with [" << gameData.orbs[i].name << " Orb]" << endl;
        }
        return;
    }
    else if (!isNumber(u) || !isNumber(o))
    {
        cout << "Invalid input" << endl;
        return;
    }
    int uIdx = stoi(u), oIdx = stoi(o);
    if (uIdx < 0 || uIdx >= gameData.units.size() || oIdx < 0 || oIdx >= gameData.orbs.size())
    {
        cout << "Unit/Orb selected is out of range" << endl;
        return;
    }

    gameData.units[uIdx].orb = gameData.orbs[oIdx];
    gameData.units[uIdx].update();
    cout << "Successfully equip [" << gameData.units[uIdx].getName() << "] with [" << gameData.orbs[oIdx].name << " Orb]" << endl;
}

void Game::inventory()
{
    string in;
    cout << " Select inventory (unit/orb/guardian): " << endl;
    cin >> in;
    gameData.inventory(in);
}

void Game::displayInfo()
{
    cout << " Select ID to display info: ";
    string i;
    cin >> i;
    cout << endl;
    if (!isNumber(i))
    {
        cout << " > Invalid input" << endl;
        return;
    }
    int idx = stoi(i);
    if (idx < 0 || idx >= gameData.units.size())
    {
        cout << " > ID selected is out of range" << endl;
        return;
    }
    cout << endl
         << " ==UNIT INFO== " << endl;
    gameData.units[idx].info();
    cout << endl
         << " ==ORB INFO== " << endl;
    gameData.orbs[idx].info();
    if (!gameData.guardians.empty() && idx >= 0 && idx < gameData.guardians.size())
    {
        cout << endl
             << " ==GUARDIAN INFO== " << endl;
        gameData.guardians[idx].info();
    }
}

void Game::gacha()
{
    cout << "Select banner (Unit/Orb): " << endl;
    string input;
    cin >> input;
    cout << endl;
    if (input == "u" || input == "unit")
    {
        Gacha<Unit> g;
        cout << " == Unit Banner == " << endl;
        cout << "Price: " << g.getPrice() << " ruby | " << " Rate: " << g.getRate() << endl;
        g.start(gameData.gold, gameData.ruby, gameData.units);
    }

    else if (input == "o" || input == "orb")
    {
        Gacha<Orb> g;
        cout << " == Orb Banner == " << endl;
        cout << "Price: " << g.getPrice() << "_ruby | " << " Rate: " << g.getRate() << endl;
        g.start(gameData.gold, gameData.ruby, gameData.orbs);
    }
}

void Game::levelUp()
{
    bool loopCond = true;
    int totalSpent = 0, prevLv = gameData.level.get();
    while (loopCond)
    {
        int goldCost = 10 * gameData.level.get();
        if (gameData.gold >= goldCost)
        {
            loopCond = gameData.level.up();
            gameData.gold -= loopCond ? goldCost : 0;
            totalSpent += loopCond ? goldCost : 0;
        }
        else
        {
            cout << " > Insufficient gold. You need " << goldCost - gameData.gold << " more" << endl;
            loopCond = false;
        }
    }
    cout << endl
         << " >> Finish leveling up. Total gold spent: " << totalSpent << " | Level increases from " << prevLv << " to " << gameData.level.get() << endl;
    gameData.setUnitLevel();
}

void Game::adminMode()
{
    string pass;
    cin >> pass;
    if (pass != "126")
        return;
    gameData.gold = 99999999;
    gameData.ruby = 99999999;
    gameData.level.setCurrentExp(99999999);
    for (Unit &u : gameData.units)
        u.setOwned();
    for (Orb &o : gameData.orbs)
        o.setOwned();
    for (Guardian &g : gameData.guardians)
        g.setOwned();
    raidSpeed = 10;
    cout << " >>> You've just accessed to Admin Mode. Get all items " << endl;
}

void Game::reloadData()
{
    const string path = gameData.config.getDataPath();
    gameData = GameData(path);
}

void Game::setRaidSpeed()
{
    cout << " Speed (1-3): " << endl;
    string in;
    cin >> in;
    if (!isNumber(in))
    {
        cout << " > Invalid input" << endl;
        return;
    }
    double s = stod(in);
    if (s == 1 || s == 2 || s == 3)
    {
        cout << " > Set speed to " << s << " successfully";
        raidSpeed = stod(in);
    }
    else
        cout << " > Speed should be from 1 to 3";
    cout << endl;
}