#include <iostream>
#include "Game.hpp"
using namespace std;

int main(){
    cout<<" VER 1.0 "<<endl;
    cout<<fixed<<setprecision(2);
    const string path = "Data/data.txt";
    Game game(path);
    game.run();
}