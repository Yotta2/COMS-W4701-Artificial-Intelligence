#ifndef BFSAGENT_H
#define BFSAGENT_H

#include <string>
#include <unordered_set>
#include <queue>
#include "../include/Puzzle.h"
#include "../include/State.h"
#include "../include/Timer.h"
//BFS agent, takes in filename and out filename

using namespace std;

class BFSAgent
{
public:
    BFSAgent(string in, string out, bool statFlag);
    void solve();
private:
    void outputSol(State &state);
    bool isDeadState(State &state, int lastMoveDir);
    bool canPushBoxToGoalAgainstWall(State &state, int lastMoveDir);
    bool clockwiseDirIsBlocked(State &state, int lastMoveDir);
    bool counterclockwiseDirIsBlocked(State &state, int lastMoveDir);
    void outputStat();
    void printState(State &state);
    string inputFilename;
    string outputFilename;
    Puzzle puzzle;
    State goalState;
    queue<State> q;
    unordered_set<State, StateKeyHash, StateKeyEqual> visitedStates;
    static int delta[4][2];// = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    static char direction[4];// = {'u', 'r', 'd', 'l'};
    Timer timer;
    int nodesGeneratedCount;
    int repeatedNodesCount;
    bool needStat;
};

#endif // BFSAGENT_H
