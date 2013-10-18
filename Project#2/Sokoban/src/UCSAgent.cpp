#include "../include/UCSAgent.h"
#include "../include/PuzzleParser.h"
#include <iostream>
#include <fstream>
#include <set>
#include "../include/Location.h"

UCSAgent::UCSAgent(string in, string out)
{
    //ctor
    inputFilename = in;
    outputFilename = out;
    puzzle = PuzzleParser::parse(inputFilename);
    goalState.pLoc = puzzle.pLoc;
    goalState.boxes = puzzle.goals;
}

void UCSAgent::outputSol(State &state) {
    ofstream ofs(outputFilename.c_str());
    ofs << "Solution:" << endl;
    for (int i = 0; i < state.prevMoves.size(); i++)
        ofs << state.prevMoves[i] << ",";
    timer.setEndTime();
    outputStat();
}

void UCSAgent::outputStat() {
    cout << "a) The number of nodes generated: " << nodesGeneratedCount << endl;
    cout << "b) The number of nodes containing states that were generated previously: " << endl;
    cout << "c) The number of nodes on the fringe when termination occurs: " << pQueue.size() << endl;
    cout << "d) The number of nodes on the explored list (if there is one) when termination occurs: " << visitedStates.size() << endl;
    cout << "e) The actual run time(millisecond): " << timer.getTimeElapsed() << endl;
}

void UCSAgent::solve() {
    timer.setStartTime();
    State currState;
    currState.pLoc = puzzle.pLoc;
    currState.boxes = puzzle.boxes;
    currState.pathCost = 0;
    currState.stateType = 'U';
    pQueue.push(currState);
    nodesGeneratedCount = 0;
    while (!pQueue.empty()) {
        currState = pQueue.top();
        pQueue.pop();
        if (currState == goalState) {
            outputSol(currState);
            return;
        }
        for (int i = 0; i < 4; i++) {
            State nextState = currState;
            nextState.pLoc.x += delta[i][0];
            nextState.pLoc.y += delta[i][1];
            nextState.pathCost++;
            nextState.stateType = 'U';
            nextState.prevMoves.push_back(direction[i]);
            if (puzzle.configuration[nextState.pLoc.x][nextState.pLoc.y] == '#')
                continue;   //hit wall, discard state
            if (nextState.boxes.find(nextState.pLoc) != nextState.boxes.end()) {    // need to push box
                Location nextDoor(nextState.pLoc.x + delta[i][0], nextState.pLoc.y + delta[i][1]);
                if (puzzle.configuration[nextDoor.x][nextDoor.y] == '#')
                    continue;   //next door is a wall, discard state
                if (nextState.boxes.find(nextDoor) == nextState.boxes.end()) {
                    nextState.boxes.erase(nextState.pLoc);
                    nextState.boxes.insert(nextDoor);
                    nextState.pathCost++;   //push a box, cost == 2
                    if (isDeadState(nextState, i))
                        continue;   // after pushing a box, enter a dead state, get discarded
                } else {
                    continue;   //next door is a box, discard state
                }
            }
            if (visitedStates.find(nextState) == visitedStates.end()) {
                visitedStates.insert(nextState);
                pQueue.push(nextState);
                nodesGeneratedCount++;
            }
        }
    }
}

// lastMoveDir and clockwiseDir has wall or box
bool UCSAgent::clockwiseDirIsBlocked(State &state, int lastMoveDir) {
    int clockwiseDir = (lastMoveDir + 1) % 4;

    Location loc0(state.pLoc.x + delta[lastMoveDir][0] * 2,
                      state.pLoc.y + delta[lastMoveDir][1] * 2);
    Location loc1(state.pLoc.x + delta[lastMoveDir][0] + delta[clockwiseDir][0],
                  state.pLoc.y + delta[lastMoveDir][1] + delta[clockwiseDir][1]);
    while (true) {
        if (puzzle.configuration[loc0.x][loc0.y] != '#'
            && state.boxes.find(loc0) == state.boxes.end())
                break;
        if (puzzle.configuration[loc1.x][loc1.y] == '#'
            || state.boxes.find(loc1) != state.boxes.end())
            return true;
        loc0.x += delta[clockwiseDir][0];
        loc0.y += delta[clockwiseDir][1];
        loc1.x += delta[clockwiseDir][0];
        loc1.y += delta[clockwiseDir][1];
    }
    return false;
}

// lastMoveDir and counterclockwiseDir has wall or box
bool UCSAgent::counterclockwiseDirIsBlocked(State &state, int lastMoveDir) {
    int counterclockwiseDir = (lastMoveDir - 1 + 4) % 4;

    Location loc0(state.pLoc.x + delta[lastMoveDir][0] * 2,
                      state.pLoc.y + delta[lastMoveDir][1] * 2);
    Location loc1(state.pLoc.x + delta[lastMoveDir][0] + delta[counterclockwiseDir][0],
                  state.pLoc.y + delta[lastMoveDir][1] + delta[counterclockwiseDir][1]);
    while (true) {
        if (puzzle.configuration[loc0.x][loc0.y] != '#'
            && state.boxes.find(loc0) == state.boxes.end())
                break;
        if (puzzle.configuration[loc1.x][loc1.y] == '#'
            || state.boxes.find(loc1) != state.boxes.end())
                return true;
        loc0.x += delta[counterclockwiseDir][0];
        loc0.y += delta[counterclockwiseDir][1];
        loc1.x += delta[counterclockwiseDir][0];
        loc1.y += delta[counterclockwiseDir][1];
    }
    return false;
}

bool UCSAgent::isDeadState(State &state, int lastMoveDir) {
    return clockwiseDirIsBlocked(state, lastMoveDir)
        && counterclockwiseDirIsBlocked(state, lastMoveDir);
}
