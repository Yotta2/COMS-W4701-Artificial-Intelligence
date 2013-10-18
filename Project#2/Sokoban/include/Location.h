#ifndef LOCATION_H
#define LOCATION_H

struct Location {
    int x, y;
    Location() {}
    Location(int _x, int _y) : x(_x), y(_y) {}
    bool operator<(const Location &rhs) const {
        if (x != rhs.x)
            return x < rhs.x;
        else if (x == rhs.x)
            return y < rhs.y;
    }
    bool operator==(const Location &rhs) const {
        return (x == rhs.x) && (y == rhs.y);
    }
};

#endif // LOCATION_H
