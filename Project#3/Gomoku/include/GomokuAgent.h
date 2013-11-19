#ifndef GOMOKUAGENT_H
#define GOMOKUAGENT_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <climits>
#include <cmath>
#include "../include/Move.h"

using namespace std;

class GomokuAgent
{
public:
    GomokuAgent(int n, int m, int s, char p);
    void kickOff();
private:
    typedef vector<string> Board;
    Board currState;
    char agentCharacter;  // 'x' or 'o'
    char opponentCharacter; // 'x' or 'o'
    int boardDimension;
    int winningChainLength;
    int timeLimit;
    unordered_set<Move, MoveKeyHash, MoveKeyEqual> remainingMoveList;
    struct Action {
        Move move;
        long long minimax;
        Action() {}
        Action(Move _move, int _minimax) : move(_move), minimax(_minimax) {}
    };
    int delta[4][2] = {{+1, 0}, {+1, +1}, {0, +1}, {-1, +1}};
private:
    Move alphaBetaSearch();
    void placePiece(Move &move, char stoneCharacter);
    void printBoard();
    void outputStartInfo();
    bool gameOver(Move &lastMove, char lastPieceType);
    bool hasUnbrokenChainOfLen(int len, Move &lastMove, char lastPieceType, Board &state);
    bool isOutOfBound(int x, int y);
    bool isIllegalMove(Move &move);
    Action maxValue(Board &state, int depth, long long alpha, long long beta);
    Action minValue(Board &state, int depth, long long alpha, long long beta);
    bool hasStoneNearby(int x, int y, Board &state);
    bool agentWon();
    bool opponentWon();
    long long evaluate(Board &state, char nextTurnChar);
    bool hasOpen(int len, char pieceType, Board &state);
    bool hasCapped(int len, char pieceType, Board &state);
    bool hasOpenStartingFrom(int x, int y, int dir, int len, char pieceType, Board &state);
    bool hasCappedStartingFrom(int x, int y, int dir, int len, char pieceType, Board &state);
};

#endif // GOMOKUAGENT_H
