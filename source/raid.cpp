#include "Raid.hpp"

bool Raid::start()
{
    if (onDeploy.getName() == "")
    {
        cout << " > Deploy unit first !" << endl;
        return false;
    }
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
        guardian.skill(round,onDeploy,enemy);
        takeAction(onDeploy, enemy);
        takeAction(enemy, onDeploy);
        cout << string(30, '-') << endl;
        onDeploy.displayStats();
        enemy.displayStats();
        this_thread::sleep_for(chrono::milliseconds(int(2000 / speed)));
        cout << string(30, '-') << endl;
        round++;
    } while (onDeploy.isAlive() && enemy.isAlive() && round < 25);
    return !enemy.isAlive();
}

void Raid::takeAction(Unit &attacker, Unit &target)
{
    attacker.updateBuffs();
    cout << attacker.getName() << "'s turn: " << endl;
    if (!attacker.isAlive() || !target.isAlive() || attacker.updateBadStatus(target))
        return;
    this_thread::sleep_for(chrono::milliseconds(int(1000 / speed)));
    if (attacker.isEnoughEnergy())
        attacker.ultimate(target);
    else
        attacker.attack(target);
    this_thread::sleep_for(chrono::milliseconds(int(1500 / speed)));
}
