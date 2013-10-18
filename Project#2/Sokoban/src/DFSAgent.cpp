#include "../include/DFSAgent.h"
#include "../include/PuzzleParser.h"
#include <iostream>
#include <fstream>
#include <set>
#include "../include/Location.h"

DFSAgent::DFSAgent(string in, string out)
{
    //ctor
    inputFilename = in;
    outputFilename = out;
    puzzle = PuzzleParser::parse(inputFilename);
    goalState.pLoc = puzzle.pLoc;
    goalState.boxes = puzzle.goals;
}

void DFSAgent::outputSol(State &state) {
    ofstream ofs(outputFilename.c_str());
    for (int i = 0; i < state.prevMoves.size(); i++)
        ofs << state.prevMoves[i] << ",";
}

void DFSAgent::solve() {
    State currState;
    bool found = false;
    currState.pLoc = puzzle.pLoc;
    currState.boxes = puzzle.boxes;
    dfs(currState, found);
}

void DFSAgent::dfs(State &currState, bool &found) {
    if (currState == goalState) {
        found = true;
        outputSol(currState);
    }
    if (found)
        return;
    for (int i = 0; i < 4; i++) {
        State nextState = currState;
        nextState.pLoc.x += delta[i][0];
        nextState.pLoc.y += delta[i][1];
        nextState.prevMoves.push_back(direction[i]);
        if (puzzle.configuration[nextState.pLoc.x][nextState.pLoc.y] == '#')
            continue;   //hit wall, discard state
        if (nextState.boxes.find(nextState.pLoc) != nextState.boxes.end()) { // need to push box
            Location nextDoor(nextState.pLoc.x + delta[i][0], nextState.pLoc.y + delta[i][1]);
            if (puzzle.configuration[nextDoor.x][nextDoor.y] == '#')
                continue;   //next door is a wall, discard state
            if (nextState.boxes.find(nextDoor) == nextState.boxes.end()) {
                nextState.boxes.erase(nextState.pLoc);
                nextState.boxes.insert(nextDoor);
            } else {
                continue;   //next door is a box, discard state
            }
        }
        if (visitedStates.find(nextState) == visitedStates.end()) {
            visitedStates.insert(nextState);
            dfs(nextState, found);
        }
    }
}
