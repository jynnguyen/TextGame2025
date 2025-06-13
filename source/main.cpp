#include <iostream>
#include "Game.hpp"
using namespace std;

int main(){
    cout<<fixed<<setprecision(2);
    cout<<" VER 1.0 "<<endl;
    const string path = "Data/data.txt";
    Game game(path);
    game.run();
}