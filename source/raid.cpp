#include "Raid.hpp"

Raid::Raid(Unit deploy, Unit e, Guardian g, double s) : onDeploy(deploy), enemy(e), guardian(g), speed(s)
{
}

bool Raid::normal()
{
    cout << " == NORMAL RAID == " << endl;
    cout << enemy.info();
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
        display();
        round++;
    } while (onDeploy.isAlive() && enemy.isAlive() && round < 25);
    cout << " Total dmg dealt (based on Enemy's max and current HP): " << enemy.stats.getHpLost() << endl;
    return !enemy.isAlive();
}

double Raid::boss()
{
    cout << " == BOSS RAID == " << endl;
    cout << enemy.info();
    int round = 0;
    do
    {
        cout << " _________" << endl;
        cout << " [ROUND " << round << "]" << endl;
        cout << " ---------" << endl;
        guardian.skill(round, onDeploy, enemy);
        takeAction(onDeploy, enemy);
        takeAction(enemy, onDeploy);
        display();
        round++;
    } while (onDeploy.isAlive() && enemy.isAlive() && round < 30);
    cout << " Total dmg dealt (based on Enemy's max and current HP): " << enemy.stats.getHpLost() << endl;
    return enemy.stats.getHpLost();
}

void Raid::takeAction(Unit &attacker, Unit &target)
{
    attacker.stats.updateBuffs();
    cout << attacker.getName() << "'s turn: " << endl;
    double dmgDealt = 0;
    if (attacker.updatePreStatus(target))
    {
        this_thread::sleep_for(chrono::milliseconds(int(1000 / speed)));
        return;
    }
    if (!attacker.isAlive())
    {
        cout << attacker.getName() << " is dead !" << endl;
        return;
    }
    if (!target.isAlive())
        return;
    this_thread::sleep_for(chrono::milliseconds(int(1000 / speed)));
    if (attacker.isEnoughEnergy())
        dmgDealt += attacker.ultimate(target);
    else
        dmgDealt += attacker.attack(target);
    if (dmgDealt > 1e-10){
        this_thread::sleep_for(chrono::milliseconds(int(1000 / speed)));
        dmgDealt += target.isCounter(attacker);
    }
    this_thread::sleep_for(chrono::milliseconds(int(1000 / speed)));
}

void Raid::displayDetail()
{
    cout << string(30, '-') << endl;
    cout << onDeploy.info();
    cout << enemy.info();
    this_thread::sleep_for(chrono::milliseconds(int(2000 / speed)));
    cout << string(30, '-') << endl;
}
void Raid::display()
{
    cout << string(50, '-') << endl;
    onDeploy.displayStats();
    cout << string(10, '-') << endl;
    enemy.displayStats();
    this_thread::sleep_for(chrono::milliseconds(int(2000 / speed)));
    cout << string(50, '-') << endl;
}