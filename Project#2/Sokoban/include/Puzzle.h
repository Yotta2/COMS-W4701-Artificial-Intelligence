#ifndef PUZZLE_H
#define PUZZLE_H

#include <set>
#include <string>
#include <vector>
#include "Location.h"

using namespace std;

struct Puzzle {
    vector<string> configuration;   //empty warehouse, only includes walls
    set<Location> goals;         //locations of goals
    set<Location> boxes;         //locations of boxes
    Location pLoc;                  //player's initial location
};


#endif // PUZZLE_H
