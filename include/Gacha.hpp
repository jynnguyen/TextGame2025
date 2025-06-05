#pragma once

#include "main.hpp"
#include "Unit.hpp"
#include "Functions.hpp"

template <typename T>
class Gacha
{
public:
    Gacha(int pR = 10, double rSr = 0.1, double rSsr = 0.06, double rUr = 0.03) : priceInRuby(pR), rateSr(rSr), rateSsr(rSsr), rateUr(rUr)
    {
        if (is_same<T, Orb>::value)
        {
            rateSr /= 2, rateSsr /= 2, rateUr /= 2;
            banner = "Orb";
        }
    };

    void start(int &gold, int &ruby, vector<T> &vecs)
    {
        int pulls = howManyPulls();
        if (!hasEnoughRuby(ruby, pulls * priceInRuby))
            return;
        while (pulls > 0)
        {
            int chance = rngRate(1 - rateSr - rateSsr - rateUr, rateSr, rateSsr, rateUr);
            int size = vecs.size()-1;
            int SSR = 5, UR = 10;
            if (chance == 1)
            {
                int idx = rng(0, size);
                while (idx % SSR == 0 || idx % UR == 0)
                    idx = rng(0, size);
                ruby += obtain(vecs, idx);
            }
            else if (chance == 2 && size >= 10)
            {
                int idx = rng(0, size);
                while (idx % SSR != 0 && idx != 0)
                    idx = rng(0, size);
                ruby += obtain(vecs, idx);
            }
            else if (chance == 3 && size >= 15)
            {
                int idx = rng(0, size);
                while (idx % UR != 0 && idx != 0)
                    idx = rng(0, size);
                ruby += obtain(vecs, idx);
            }
            else
            {
                int extraGold = rng(10, 20);
                int extraRuby = rng(1, 5);
                cout << " > You get " << extraGold << "_gold and " << extraRuby << "_ruby ";
            }
            ruby -= priceInRuby;
            pulls--;
            cout << endl;
        }
    }

    int getPrice()
    {
        return priceInRuby;
    }

    string getRate()
    {
        stringstream ss;
        ss<<"SR = "<<rateSr*100<<"% | SSR = "<<rateSsr*100<<"% | UR = "<<rateUr*100<<"%"<<endl;
        return ss.str();
    }

private:
    int priceInRuby;
    double rateSr, rateSsr, rateUr;
    string banner = "Unit";
    int howManyPulls()
    {
        string input;
        cout << " >>|" << banner << " Banner|<< " << endl;
        cout << "Enter number of pulls (1-10): " << endl;
        cin >> input;
        if (!isNumber(input))
            return -1;
        int pulls = stoi(input);
        return (pulls >= 1 && pulls <= 10) ? pulls : -1;
    }

    int obtain(vector<T> &vecs, int idx)
    {
        if (idx < 0 || idx >= vecs.size())
            return 0;
        cout << " > Congrats ! You get a " << vecs[idx].getRarity() << " " << banner << ", [" << vecs[idx].getName() << "]";
        if (vecs[idx].getOwned())
        {
            int returnRuby = priceInRuby * 2;
            cout << " (You already owned this. Refund " << returnRuby << "_ruby)";
            return returnRuby;
        }
        else
            vecs[idx].setOwned();
        return 0;
    }

    bool hasEnoughRuby(int &ruby, const int &rubyCost)
    {
        bool result = ruby >= rubyCost;
        if (!result)
            cout << " > Insufficient ruby. You need " << rubyCost - ruby << " more ruby";
        return result;
    }
};