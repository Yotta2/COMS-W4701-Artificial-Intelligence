#include <iostream>
#include "../include/GomokuAgent.h"

using namespace std;

int main() {
    int n, m, s;
    char player;
    cout << "Welcome to Gomoku!!!" << endl;
    cout << "Please input the game dimension n: ";
    cin >> n;
    cout << "Please input the winning chain length m: ";
    cin >> m;
    cout << "Please input move selection time limit s(in seconds): ";
    cin >> s;
    cout << "Please specify the agent will play x or o: ";
    cin >> player;

    GomokuAgent gomokuAgent(n, m, s, player);
    gomokuAgent.kickOff();
    return 0;
}
