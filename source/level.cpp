#include "Level.hpp"

int Level::get()
{
    return level;
}

void Level::set(int lv)
{
    level = lv;
}

void Level::setCurrentExp(int amount)
{
    currentExp = amount;
}

int Level::getCurrentExp()
{
    return currentExp;
}

void Level::info()
{
    cout << " >>[Level Info]<< " << endl;
    cout << "Level: " << level << " | Exp: " << currentExp << "/" << requiredExp << endl;
}

bool Level::up()
{
    if(level >= levelCap){
        cout << " >> You've reached MAX level " << level <<endl;
        return false;
    }
    else if (hasEnoughExp())
    {
        currentExp -= requiredExp;
        set(level + 1);
        setRequiredExp();
        cout << " > You've reached level " << level <<endl;
        return true;
    }
    else
    {
        cout << " > Insufficient Exp. You need " << requiredExp - currentExp << " more"<<endl;
        return false;
    }
}

void Level::setRequiredExp()
{
    requiredExp = 10 * level;
}

bool Level::hasEnoughExp()
{
    return currentExp >= requiredExp;
}
