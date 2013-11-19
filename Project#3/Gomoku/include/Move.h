#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

using namespace std;

struct Move {
    int x, y;
    Move() {}
    Move(int _x, int _y) : x(_x), y(_y) {}
};

class MoveKeyHash {
public:
    size_t operator () (const Move &move) const {
        size_t hashcode = (hash<int>()(move.x)) ^ ((hash<int>()(move.y)) << 3);
        return hashcode;
    }
};

class MoveKeyEqual {
public:
    bool operator () (const Move &move0, const Move &move1) const {
        return (move0.x == move1.x && move0.y == move1.y);
    }
};

#endif // MOVE_H_INCLUDED
