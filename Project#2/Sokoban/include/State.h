#ifndef STATE_H
#define STATE_H

#include "Location.h"
#include <set>
#include <vector>

using namespace std;

struct State {
    Location pLoc;                      //player's current location
    set<Location> boxes;          //locations of boxes
    vector<char> prevMoves;             //previous moves
    int pathCost;                   //path cost from initial state to current state, for UCS and A*
    int estimatedTotalCost;         //f(n) = g(n) + h(n)
    int inGoalCount;                //describe the desirability for GBFS
    char stateType;                   // flag to mark state type, 'U' for UCS, 'G' for GBFS, 'A' for ASTARS
    bool operator==(const State &rhs) const {
        return pLoc == rhs.pLoc && boxes == rhs.boxes;
//        set<Location>::iterator lItr = boxes.begin();
//        set<Location>::iterator rItr = rhs.boxes.begin();
//        while (lItr != boxes.end()) {
//            if ((*lItr) != (*rItr))
//                return false;
//            lItr++;
//            rItr++;
//        }
//        return true;
    }
    bool operator<(const State &rhs) const {
        if (stateType == 'U')
            return pathCost < rhs.pathCost;
        if (stateType == 'G')
            return inGoalCount < rhs.inGoalCount;
        if (stateType == 'A')
            return estimatedTotalCost < rhs.estimatedTotalCost;
    }
    bool operator>(const State &rhs) const {
        if (stateType == 'U')
            return pathCost > rhs.pathCost;
        if (stateType == 'G')
            return inGoalCount > rhs.inGoalCount;
        if (stateType == 'A')
            return estimatedTotalCost > rhs.estimatedTotalCost;
    }
};


class StateKeyHash
{
public:
    size_t operator () (const State &state) const {
        Location pLoc = state.pLoc;
        size_t hashcode = (hash<int>()(pLoc.x)) ^ ((hash<int>()(pLoc.y)) << 5);
        int shift = 2;
        for (set<Location>::iterator itr = state.boxes.begin();
                itr != state.boxes.end(); itr++) {
            hashcode ^= (hash<int>()(itr->x)) ^ ((hash<int>()(itr->y)) << shift);
            hashcode <<= shift;
            shift = (shift + 5) % 32;
        }

        return hashcode;
    }
};

class StateKeyEqual {
public:
    bool operator () (const State &state1, const State &state2) const {
        if (state1.pLoc == state2.pLoc && state1.boxes == state2.boxes)
            return true;
        else
            return false;
    }
};

#endif // STATE_H
