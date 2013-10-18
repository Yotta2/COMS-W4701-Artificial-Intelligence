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
    for (int i = 0; i < state.prevMoves.size(); i++)
        ofs << state.prevMoves[i] << ",";
}

void UCSAgent::solve() {
    State currState;
    currState.pLoc = puzzle.pLoc;
    currState.boxes = puzzle.boxes;
    currState.pathCost = 0;
    currState.stateType = 'U';
    pQueue.push(currState);
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
                } else {
                    continue;   //next door is a box, discard state
                }
            }
            if (visitedStates.find(nextState) == visitedStates.end()) {
                visitedStates.insert(nextState);
                pQueue.push(nextState);
            }
        }
    }
}
