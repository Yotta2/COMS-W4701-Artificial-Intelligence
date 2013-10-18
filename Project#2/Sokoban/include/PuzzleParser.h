#ifndef PUZZLEPARSER_H
#define PUZZLEPARSER_H

#include "Puzzle.h"

using namespace std;

class PuzzleParser
{
public:
    PuzzleParser();
    static Puzzle parse(string filename); //given an input filename, parse the file and return a puzzle object
private:
    static void parseLine(int i, string &line, Puzzle &puzzle);
};

#endif // PUZZLEPARSER_H
