#include "../include/PuzzleParser.h"
#include <string>
#include <fstream>
#include <iostream>

PuzzleParser::PuzzleParser()
{
    //ctor
}

Puzzle PuzzleParser::parse(string filename) {
    ifstream ifs(filename.c_str());
    Puzzle puzzle;
    int lineCount = 0;
    ifs >> lineCount;
    puzzle.configuration.resize(lineCount);
    int i = 0;
    string line;
    getline(ifs, line); //consume a '/r'
    while (i < lineCount) {
        getline(ifs, line);
        //std::cout << line << endl;
        parseLine(i, line, puzzle);
        i++;
    }
    return puzzle;
}

//parse ith line
//#	(hash)  	Wall
//.	(period)	Empty goal
//@	(at)    	Player on floor
//+	(plus)		Player on goal
//$	(dollar)	Box on floor
//*	(asterisk)	Box on goal
void PuzzleParser::parseLine(int i, string &line, Puzzle &puzzle) {
    for (int j = 0; j < line.size(); j++) {
        switch (line[j]) {
        case '.':
            puzzle.goals.insert(Location(i, j));
            break;
        case '@':
            puzzle.pLoc.x = i;
            puzzle.pLoc.y = j;
            break;
        case '+':
            puzzle.pLoc.x = i;
            puzzle.pLoc.y = j;
            puzzle.goals.insert(Location(i, j));
            break;
        case '$':
            puzzle.boxes.insert(Location(i, j));
            break;
        case '*':
            puzzle.boxes.insert(Location(i, j));
            puzzle.goals.insert(Location(i, j));
            break;
        }
        if (line[j] != '#')
            line[j] = ' ';
        puzzle.configuration[i] = line;
    }
}
