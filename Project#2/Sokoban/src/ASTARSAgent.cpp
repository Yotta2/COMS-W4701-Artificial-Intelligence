#include "../include/ASTARSAgent.h"
#include "../include/PuzzleParser.h"
#include <iostream>
#include <fstream>
#include <set>
#include <cmath>
#include "../include/Location.h"

ASTARSAgent::ASTARSAgent(string in, string out)
{
    //ctor
    inputFilename = in;
    outputFilename = out;
    puzzle = PuzzleParser::parse(inputFilename);
    goalState.pLoc = puzzle.pLoc;
    goalState.boxes = puzzle.goals;
}

void ASTARSAgent::outputSol(State &state) {
    ofstream ofs(outputFilename.c_str());
    for (int i = 0; i < state.prevMoves.size(); i++)
        ofs << state.prevMoves[i] << ",";
}

void ASTARSAgent::solve() {
    State currState;
    currState.pLoc = puzzle.pLoc;
    currState.boxes = puzzle.boxes;
    currState.pathCost = 0;
    currState.estimatedTotalCost = currState.pathCost + heuristic(currState);
    currState.stateType = 'A';
    pQueue.push(currState);
    while (!pQueue.empty()) {
        currState = pQueue.top();
        pQueue.pop();
        for (int i = 0; i < 4; i++) {
            State nextState = currState;
            nextState.pLoc.x += delta[i][0];
            nextState.pLoc.y += delta[i][1];
            nextState.pathCost++;
            nextState.estimatedTotalCost = nextState.pathCost + heuristic(nextState);
            nextState.stateType = 'A';
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
                } else {
                    continue;   //next door is a box, discard state
                }
            }
            if (nextState == goalState) {
                outputSol(nextState);
                return;
            }
            if (visitedStates.find(nextState) == visitedStates.end()) {
                visitedStates.insert(nextState);
                pQueue.push(nextState);
            }
        }
    }
}

int ASTARSAgent::heuristic(State &state) {  // cost from current location to the farthest uncovered goal location
    int farthestDist = 0;
    for (set<Location>::iterator itr = puzzle.goals.begin(); itr != puzzle.goals.end(); itr++)
        if (state.boxes.find(*itr) == state.boxes.end()) {
            int tmp = computeDist(state.pLoc, *itr);
            if (tmp > farthestDist)
                farthestDist = tmp;
        }
    return farthestDist;
}

int ASTARSAgent::computeDist(const Location &loc1, const Location &loc2) {
    return abs(loc1.x - loc2.x) + abs(loc1.y - loc2.y);
}



