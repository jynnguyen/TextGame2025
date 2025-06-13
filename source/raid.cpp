#include "Raid.hpp"

Raid::Raid(Unit deploy, Unit e, Guardian g, double s) : onDeploy(deploy), enemy(e), guardian(g), speed(s)
{
}


bool Raid::normal()
{
    cout << " == NORMAL RAID == " << endl;
    enemy.info();
    string input;
    cout << endl
         << " Select floor: ";
    cin >> input;
    cout << endl;
    if (!isNumber(input))
    {
        cout << " > Invalid floor";
        return false;
    }
    enemy.setLevel(stoi(input));
    int round = 0;
    do
    {
        cout << " _________" << endl;
        cout << " [ROUND " << round << "]" << endl;
        cout << " ---------" << endl;
        guardian.skill(round, onDeploy, enemy);
        takeAction(onDeploy, enemy);
        takeAction(enemy, onDeploy);
        cout << string(30, '-') << endl;
        onDeploy.displayStats();
        enemy.displayStats();
        this_thread::sleep_for(chrono::milliseconds(int(2000 / speed)));
        cout << string(30, '-') << endl;
        round++;
    } while (onDeploy.isAlive() && enemy.isAlive() && round < 25);
    cout << " Total dmg dealt (based on Enemy's max and current HP): " << enemy.stats.getHpLost() << endl;
    return !enemy.isAlive();
}

double Raid::boss()
{
    cout << " == BOSS RAID == " << endl;
    enemy.info();
    int round = 0;
    do
    {
        cout << " _________" << endl;
        cout << " [ROUND " << round << "]" << endl;
        cout << " ---------" << endl;
        guardian.skill(round, onDeploy, enemy);
        takeAction(onDeploy, enemy);
        takeAction(enemy, onDeploy);
        cout << string(30, '-') << endl;
        onDeploy.displayStats();
        enemy.displayStats();
        this_thread::sleep_for(chrono::milliseconds(int(2000 / speed)));
        cout << string(30, '-') << endl;
        round++;
    } while (onDeploy.isAlive() && enemy.isAlive() && round < 25);
    cout << " Total dmg dealt (based on Enemy's max and current HP): " << enemy.stats.getHpLost() << endl;
    return enemy.stats.getHpLost();
}

void Raid::takeAction(Unit &attacker, Unit &target)
{
    attacker.stats.updateBuffs();
    cout << attacker.getName() << "'s turn: " << endl;
    if (attacker.updateBadStatus(target) || !attacker.isAlive() || !target.isAlive())
        return;
    this_thread::sleep_for(chrono::milliseconds(int(1000 / speed)));
    if (attacker.isEnoughEnergy())
        attacker.ultimate(target);
    else
        attacker.attack(target);
    this_thread::sleep_for(chrono::milliseconds(int(1500 / speed)));
}
