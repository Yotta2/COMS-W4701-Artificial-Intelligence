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

int DFSAgent::delta[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
char DFSAgent::direction[4] = {'u', 'r', 'd', 'l'};

void DFSAgent::outputSol(State &state) {
    ofstream ofs(outputFilename.c_str());
    ofs << "Solution:" << endl;
    for (int i = 0; i < state.prevMoves.size(); i++)
        ofs << state.prevMoves[i] << ",";
    timer.setEndTime();
    outputStat();
}

void DFSAgent::outputStat() {
    cout << "a) The number of nodes generated: " << nodesGeneratedCount << endl;
    cout << "b) The number of nodes containing states that were generated previously: " << repeatedNodesCount << endl;
    cout << "c) The number of nodes on the fringe when termination occurs: " << 0 << endl;
    cout << "d) The number of nodes on the explored list (if there is one) when termination occurs: " << visitedStates.size() << endl;
    cout << "e) The actual run time(millisecond): " << timer.getTimeElapsed() << endl;
}

void DFSAgent::solve() {
    timer.setStartTime();
    State currState;
    bool found = false;
    currState.pLoc = puzzle.pLoc;
    currState.boxes = puzzle.boxes;
    nodesGeneratedCount = 0;
    repeatedNodesCount = 0;
    dfs(currState, found);
}

void DFSAgent::dfs(State &currState, bool &found) {
    if (currState.boxes == goalState.boxes) {
        found = true;
        outputSol(currState);
    }
    if (found)
        return;
    for (int i = 0; i < 4; i++) {
        nodesGeneratedCount++;
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
                //if (isDeadState(nextState, i))
                //        continue;   // after pushing a box, enter a dead state, get discarded
            } else {
                continue;   //next door is a box, discard state
            }
        }
        if (visitedStates.find(nextState) == visitedStates.end()) {
            visitedStates.insert(nextState);
            dfs(nextState, found);
        } else {
            repeatedNodesCount++;
        }
    }
}

// lastMoveDir and clockwiseDir has wall or box
bool DFSAgent::clockwiseDirIsBlocked(State &state, int lastMoveDir) {
    int clockwiseDir = (lastMoveDir + 1) % 4;

    Location loc0(state.pLoc.x + delta[lastMoveDir][0] * 2,
                  state.pLoc.y + delta[lastMoveDir][1] * 2);
    Location loc1(state.pLoc.x + delta[lastMoveDir][0] + delta[clockwiseDir][0],
                  state.pLoc.y + delta[lastMoveDir][1] + delta[clockwiseDir][1]);
    while (true) {
        if (puzzle.configuration[loc0.x][loc0.y] != '#')
            //&& state.boxes.find(loc0) == state.boxes.end())
                break;
        if (puzzle.configuration[loc1.x][loc1.y] == '#')
            //|| state.boxes.find(loc1) != state.boxes.end())
            return true;
        loc0.x += delta[clockwiseDir][0];
        loc0.y += delta[clockwiseDir][1];
        loc1.x += delta[clockwiseDir][0];
        loc1.y += delta[clockwiseDir][1];
    }
    return false;
}

// lastMoveDir and counterclockwiseDir has wall or box
bool DFSAgent::counterclockwiseDirIsBlocked(State &state, int lastMoveDir) {
    int counterclockwiseDir = (lastMoveDir - 1 + 4) % 4;

    Location loc0(state.pLoc.x + delta[lastMoveDir][0] * 2,
                  state.pLoc.y + delta[lastMoveDir][1] * 2);
    Location loc1(state.pLoc.x + delta[lastMoveDir][0] + delta[counterclockwiseDir][0],
                  state.pLoc.y + delta[lastMoveDir][1] + delta[counterclockwiseDir][1]);
    while (true) {
        if (puzzle.configuration[loc0.x][loc0.y] != '#')
            //&& state.boxes.find(loc0) == state.boxes.end())
                break;
        if (puzzle.configuration[loc1.x][loc1.y] == '#')
            //|| state.boxes.find(loc1) != state.boxes.end())
                return true;
        loc0.x += delta[counterclockwiseDir][0];
        loc0.y += delta[counterclockwiseDir][1];
        loc1.x += delta[counterclockwiseDir][0];
        loc1.y += delta[counterclockwiseDir][1];
    }
    return false;
}

bool DFSAgent::canPushBoxToGoalAgainstWall(State &state, int lastMoveDir) {
    int clockwiseDir = (lastMoveDir + 1) % 4;
    int counterclockwiseDir = (lastMoveDir - 1 + 4) % 4;

    // try push the box in clockwise direction
    Location pLoc0(state.pLoc.x + delta[lastMoveDir][0] + delta[counterclockwiseDir][0],
                   state.pLoc.y + delta[lastMoveDir][1] + delta[counterclockwiseDir][1]);
    if (puzzle.configuration[pLoc0.x][pLoc0.y] != '#'
        && state.boxes.find(pLoc0) == state.boxes.end()) { // possible to go required location to push the box
        Location boxLoc(state.pLoc.x + delta[lastMoveDir][0],
                        state.pLoc.y + delta[lastMoveDir][1]);
        while (true) {
            if (puzzle.goals.find(boxLoc) != puzzle.goals.end())
                return true;
            boxLoc.x += delta[clockwiseDir][0];
            boxLoc.y += delta[clockwiseDir][1];
            if (puzzle.configuration[boxLoc.x][boxLoc.y] == '#'
                || state.boxes.find(boxLoc) != state.boxes.end())
                    break;
        }
    }

    // try push the box in counterclockwise direction
    Location pLoc1(state.pLoc.x + delta[lastMoveDir][0] + delta[clockwiseDir][0],
                   state.pLoc.y + delta[lastMoveDir][1] + delta[clockwiseDir][1]);
    if (puzzle.configuration[pLoc1.x][pLoc1.y] != '#'
        && state.boxes.find(pLoc1) == state.boxes.end()) { // possible to go required location to push the box
        Location boxLoc(state.pLoc.x + delta[lastMoveDir][0],
                        state.pLoc.y + delta[lastMoveDir][1]);
        while (true) {
            if (puzzle.goals.find(boxLoc) != puzzle.goals.end())
                return true;
            boxLoc.x += delta[counterclockwiseDir][0];
            boxLoc.y += delta[counterclockwiseDir][1];
            if (puzzle.configuration[boxLoc.x][boxLoc.y] == '#'
                || state.boxes.find(boxLoc) != state.boxes.end())
                    break;
        }
    }

    return false;
}

bool DFSAgent::isDeadState(State &state, int lastMoveDir) {
    if (canPushBoxToGoalAgainstWall(state, lastMoveDir))
        return false;
//    Location boxLoc(state.pLoc.x + delta[lastMoveDir][0],
//                    state.pLoc.y + delta[lastMoveDir][1]);
//    if (puzzle.goals.find(boxLoc) != puzzle.goals.end())
//        return false;
    return clockwiseDirIsBlocked(state, lastMoveDir)
        && counterclockwiseDirIsBlocked(state, lastMoveDir);
}
