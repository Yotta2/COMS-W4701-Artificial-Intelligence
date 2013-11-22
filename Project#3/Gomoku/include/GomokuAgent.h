#ifndef GOMOKUAGENT_H
#define GOMOKUAGENT_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <climits>
#include <cmath>
#include "../include/Move.h"

using namespace std;

class GomokuAgent
{
public:
    GomokuAgent(int n, int m, int s, char p, char _mode);
    void kickOff();
    Move alphaBetaSearch();
    Move getRandomMove();
    void placePiece(Move &move, char stoneCharacter);
    void printBoard();
    bool gameOver(Move &lastMove, char lastPieceType);
    char agentCharacter;  // 'x' or 'o'
    char opponentCharacter; // 'x' or 'o'
private:
    static string PIPE_NAME;
    typedef vector<string> Board;
    Board currState;
    char mode; // d for dumb mode(use random strategy), s for smart mode(use alpha beta search)
    ofstream ofs;
    int boardDimension;
    int winningChainLength;
    int timeLimit;
    unordered_set<Move, MoveKeyHash, MoveKeyEqual> remainingMoveList;
    struct Action {
        Move move;
        long long minimax;
        Action() {}
        Action(Move _move, long long _minimax) : move(_move), minimax(_minimax) {}
    };
    //int delta[4][2] = {{+1, 0}, {+1, +1}, {0, +1}, {-1, +1}};
    static int delta[8][2];
private:
    void writeToPipe(Move &move);
    void outputStartInfo();
    bool hasUnbrokenChainOfLen(int len, Move &lastMove, char lastPieceType, Board &state);
    bool isOutOfBound(int x, int y);
    bool isIllegalMove(Move &move);
    Action maxValue(Board &state, int depth, long long alpha, long long beta);
    Action minValue(Board &state, int depth, long long alpha, long long beta);
    bool hasStoneNearby(int x, int y, Board &state);
    bool agentWon(Move &lastMove, char lastPieceType, Board &state);
    bool opponentWon(Move &lastMove, char lastPieceType, Board &state);
    long long evaluate(Board &state, char nextTurnChar);
    bool hasOpen(int len, char pieceType, Board &state);
    bool hasCapped(int len, char pieceType, Board &state);
    bool hasOpenStartingFrom(int x, int y, int dir, int len, char pieceType, Board &state);
    bool hasCappedStartingFrom(int x, int y, int dir, int len, char pieceType, Board &state);
};

#endif // GOMOKUAGENT_H
