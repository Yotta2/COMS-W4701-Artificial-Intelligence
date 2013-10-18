#include "../include/BFSAgent.h"
#include "../include/PuzzleParser.h"
#include <iostream>
#include <fstream>
#include <set>
#include "../include/Location.h"

BFSAgent::BFSAgent(string in, string out)
{
    //ctor
    inputFilename = in;
    outputFilename = out;
    puzzle = PuzzleParser::parse(inputFilename);
//    for (int i = 0; i < puzzle.configuration.size(); i++)
//        cout << puzzle.configuration[i] << endl;
//    for (set<Location>::iterator itr = puzzle.boxes.begin(); itr != puzzle.boxes.end(); itr++)
//        cout << itr->x << " " << itr->y << endl;
    goalState.pLoc = puzzle.pLoc;
    goalState.boxes = puzzle.goals;
}

void BFSAgent::solve() {
    State currState;
    currState.pLoc = puzzle.pLoc;
    currState.boxes = puzzle.boxes;
    q.push(currState);
    while (!q.empty()) {
        currState = q.front();
        q.pop();
        for (int i = 0; i < 4; i++) {
            State nextState = currState;
            nextState.pLoc.x += delta[i][0];
            nextState.pLoc.y += delta[i][1];
            nextState.prevMoves.push_back(direction[i]);
            if (puzzle.configuration[nextState.pLoc.x][nextState.pLoc.y] == '#')
                continue;   //hit wall, discard state
            if (nextState.boxes.find(nextState.pLoc) != nextState.boxes.end()) {
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
                q.push(nextState);
            }
        }
    }
}

void BFSAgent::outputSol(State &state) {
    ofstream ofs(outputFilename.c_str());
    for (int i = 0; i < state.prevMoves.size(); i++)
        ofs << state.prevMoves[i] << ",";
}
