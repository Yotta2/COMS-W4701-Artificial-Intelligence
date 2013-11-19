#include <iostream>
#include "../include/GomokuAgent.h"

using namespace std;

void mode0(int n, int m, int s, char player) {
    //cout << "Please specify the agent mode, 'd' for dumb mode(use random strategy)" << endl;
    //cout << "'s' for smart mode(use alpha beta search): ";

    GomokuAgent gomokuAgent(n, m, s, player, 's');
    gomokuAgent.kickOff();
}

void mode1(int n, int m, int s, char player) {
    GomokuAgent agent0(n, m, s, 'x', 's');
    GomokuAgent agent1(n, m, s, 'o', 'd');

    cout << "Game kicks off..." << endl;
    Move agent0Move;
    Move agent1Move;
    while (true) {
        cout << "Agent0's turn: " << endl;
        agent0Move = agent0.alphaBetaSearch();
        cout << "I choose move: " << agent0Move.x << " " << agent0Move.y << endl;
        agent0.placePiece(agent0Move, 'x');
        agent1.placePiece(agent0Move, 'x');
        agent0.printBoard();
        if (agent0.gameOver(agent0Move, agent0.agentCharacter))
            break;
        cout << "Agent1's turn: " << endl;
        agent1Move = agent1.getRandomMove();
        cout << "I choose move: " << agent1Move.x << " " << agent1Move.y << endl;
        agent0.placePiece(agent1Move, 'o');
        agent1.placePiece(agent1Move, 'o');
        agent1.printBoard();
        if (agent1.gameOver(agent1Move, agent1.agentCharacter))
            break;
    }
}

void mode2(int n, int m, int s, char player) {
    GomokuAgent agent0(n, m, s, 'x', 's');
    GomokuAgent agent1(n, m, s, 'o', 's');

    cout << "Game kicks off..." << endl;
    Move agent0Move;
    Move agent1Move;
    while (true) {
        cout << "Agent0's turn: " << endl;
        agent0Move = agent0.alphaBetaSearch();
        cout << "I choose move: " << agent0Move.x << " " << agent0Move.y << endl;
        agent0.placePiece(agent0Move, 'x');
        agent1.placePiece(agent0Move, 'x');
        agent0.printBoard();
        if (agent0.gameOver(agent0Move, agent0.agentCharacter))
            break;
        cout << "Agent1's turn: " << endl;
        agent1Move = agent1.alphaBetaSearch();
        cout << "I choose move: " << agent1Move.x << " " << agent1Move.y << endl;
        agent0.placePiece(agent1Move, 'o');
        agent1.placePiece(agent1Move, 'o');
        agent1.printBoard();
        if (agent1.gameOver(agent1Move, agent1.agentCharacter))
            break;
    }
}

int main() {
    int n, m, s;
    char player;
    int mode;
    cout << "Welcome to Gomoku!!!" << endl;
    cout << "Please input the game dimension n: ";
    cin >> n;
    cout << "Please input the winning chain length m: ";
    cin >> m;
    cout << "Please input move selection time limit s(in seconds): ";
    cin >> s;
    cout << "Please specify the agent will play x or o: ";
    cin >> player;
    cout << "Please specify mode(0, 1, or 2): ";
    cin >> mode;
    if (mode == 0)
        mode0(n, m, s, player);
    if (mode == 1)
        mode1(n, m, s, player);
    if (mode == 2)
        mode2(n, m, s, player);

    return 0;
}
