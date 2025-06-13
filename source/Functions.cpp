#include "Functions.hpp"

int rng(int from, int to)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(from, to);
    return dist(gen);
}

int rngRate(double zero, double one, double two, double three)
{
    vector<double> rate{zero, one, two, three};
    random_device rd;
    mt19937 gen(rd());
    discrete_distribution<> dist(rate.begin(), rate.end());
    return dist(gen);
}

bool isNumber(const string &number)
{
    for (int i = 0; i < number.length(); i++)
    {
        if (!(isdigit(number[i]) || number[i] == '-'))
            return false;
    }
    return true;
}

void limit(double &value, double min, double max)
{
    if (value > max)
        value = max;
    else if (value < min)
        value = min;
}

string formatDouble(const double& value, int precision)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}