#pragma once
#include "main.hpp"

using namespace std;

int rng(int from, int to);
int rngRate(double zero, double one, double two = 0, double three = 0);
bool isNumber(const string &number);

template <typename T>
void printInventory(const vector<T> &items, const string &title,
                    function<int(const T)> getId,
                    function<string(const T)> getName,
                    function<bool(const T)> getOwned)
{
    cout << "  == " << title << " INVENTORY ==  \n";
    cout << left
         << setw(15) << "Name"
         << setw(10) << "Owned" << '\n';
    cout << string(30, '-') << '\n';

    for (const T& item : items)
    {
        cout << left
             << setw(5) << getId(item)
             << setw(15) << getName(item)
             << setw(10) << (getOwned(item) ? "Yes" : "No") << '\n';
    }
}

double getLimit(const double &value, double min, double max);