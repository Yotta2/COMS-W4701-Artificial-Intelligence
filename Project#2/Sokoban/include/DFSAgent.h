#ifndef DFSAGENT_H
#define DFSAGENT_H

#include <string>
#include <unordered_set>
#include "../include/Puzzle.h"
#include "../include/State.h"
//DFS agent, takes in filename and out filename

using namespace std;

class DFSAgent
{
public:
    DFSAgent(string in, string out);
    void solve();
private:
    void outputSol(State &state);
    void dfs(State &currState, bool &found);
    string inputFilename;
    string outputFilename;
    Puzzle puzzle;
    State goalState;
    unordered_set<State, StateKeyHash, StateKeyEqual> visitedStates;
    int delta[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    char direction[4] = {'u', 'r', 'd', 'l'};
};

#endif // DFSAGENT_H
