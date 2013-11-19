#include <iostream>
#include "../include/GomokuAgent.h"

using namespace std;

int main() {
    int n, m, s;
    char player;
    char mode;
    cout << "Welcome to Gomoku!!!" << endl;
    cout << "Please input the game dimension n: ";
    cin >> n;
    cout << "Please input the winning chain length m: ";
    cin >> m;
    cout << "Please input move selection time limit s(in seconds): ";
    cin >> s;
    cout << "Please specify the agent will play x or o: ";
    cin >> player;
    cout << "Please specify the agent mode, 'd' for dumb mode(use random strategy)" << endl;
    cout << "'s' for smart mode(use alpha beta search): ";
    cin >> mode;
    GomokuAgent gomokuAgent(n, m, s, player, mode);
    gomokuAgent.kickOff();
    return 0;
}
