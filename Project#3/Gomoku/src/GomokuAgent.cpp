#include "../include/GomokuAgent.h"
#include <cstdlib>

GomokuAgent::GomokuAgent(int n, int m, int s, char p, char _mode) {
    //ctor
    boardDimension = n;
    winningChainLength = m;
    timeLimit = s;
    agentCharacter = p;
    if (p == 'x')
        opponentCharacter = 'o';
    else
        opponentCharacter = 'x';
    mode = _mode;
    currState.assign(boardDimension, string(boardDimension, '.'));
    remainingMoveList.clear();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            Move move(i, j);
            remainingMoveList.insert(move);
        }
    //printBoard();
    ofs.open(PIPE_NAME, std::ofstream::out | std::ofstream::app);
}

void GomokuAgent::kickOff() {
    cout << "Game kicks off..." << endl;
    outputStartInfo();
    Move opponentMove;
    Move agentMove;
    while (true) {
        cout << "Please input your move: ";
        cin >> opponentMove.x >> opponentMove.y;
        if (isIllegalMove(opponentMove))
            break;
        placePiece(opponentMove, opponentCharacter);
        printBoard();
        if (gameOver(opponentMove, opponentCharacter))
            break;
        if (mode == 's')
            agentMove = alphaBetaSearch();
        else
            agentMove = getRandomMove();
        cout << "I choose move: " << agentMove.x << " " << agentMove.y << endl;
        writeToPipe(agentMove);
        placePiece(agentMove, agentCharacter);
        printBoard();
        if (gameOver(agentMove, agentCharacter))
            break;
    }
}

void GomokuAgent::writeToPipe(Move &move) {
    ofs << move.x << move.y << endl;
}

Move GomokuAgent::getRandomMove() {
    unordered_set<Move, MoveKeyHash, MoveKeyEqual>::iterator itr = remainingMoveList.begin();
    int randomNum = rand() % remainingMoveList.size();
    Move move;
    for (int i = 0; i < randomNum; i++) {
        itr++;
        move = *itr;
    }
    return move;
}

bool GomokuAgent::isIllegalMove(Move &move) {
    if (remainingMoveList.find(move) == remainingMoveList.end()) {
        cout << "Sorry, you enter an illegal move which is already occupied. You lost!" << endl;
        return true;
    }
    return false;
}

void GomokuAgent::outputStartInfo() {
    if (agentCharacter == 'x') {
        cout << "I am player x and move first!" << endl;
        Move move(boardDimension / 2, boardDimension / 2);
        placePiece(move, 'x');
        cout << "I choose move: " << move.x << " " << move.y << endl;
        printBoard();
    } else {
        cout << "I am player o and move second!" << endl;
        printBoard();
    }
}

void GomokuAgent::placePiece(Move &move, char stoneCharacter) {
    currState[move.x][move.y] = stoneCharacter;
    remainingMoveList.erase(move);
}

bool GomokuAgent::gameOver(Move &lastMove, char lastPieceType) {
    if (remainingMoveList.empty()) {
        cout << "The game board is completely full of stones. The game ended in a draw" << endl;
        return true;
    }
    if (hasUnbrokenChainOfLen(winningChainLength, lastMove, lastPieceType, currState)) {
        cout << "Player " << lastPieceType << " has created an unbroken chain of " << winningChainLength
             << ". Player " << lastPieceType << " wins!" << endl;
        return true;
    }
    return false;
}

//test whether after move lastMove, an unbroken chain of len is formed
bool GomokuAgent::hasUnbrokenChainOfLen(int len, Move &lastMove, char lastPieceType, Board &state) {
    // try all 4 direcions
    for (int i = 0; i < 4; i++) {
        int count = 1;
        int x = lastMove.x;
        int y = lastMove.y;

        // expand in both directions
        int xNext = x - delta[i][0];
        int yNext = y - delta[i][1];
        while (true) {
            if (isOutOfBound(xNext, yNext))
                break;
            if (currState[xNext][yNext] != lastPieceType)
                break;
            count++;
            xNext -= delta[i][0];
            yNext -= delta[i][1];
        }

        xNext = x + delta[i][0];
        yNext = y + delta[i][1];
        while (true) {
            if (isOutOfBound(xNext, yNext))
                break;
            if (currState[xNext][yNext] != lastPieceType)
                break;
            count++;
            xNext += delta[i][0];
            yNext += delta[i][1];
        }
        if (count >= len)
            return true;
    }
    return false;
}

bool GomokuAgent::isOutOfBound(int x, int y) {
    return (x < 0 || x >= boardDimension || y < 0 || y >= boardDimension);
}

void GomokuAgent::printBoard() {
    cout << "Now the game board looks like the following:" << endl;
    for (int i = 0; i < boardDimension; i++) {
        for (int j = 0; j < boardDimension; j++)
            cout << currState[i][j];
        cout << endl;
    }
}

Move GomokuAgent::alphaBetaSearch() {
    int depth = 5;
    Action bestAction = maxValue(currState, depth, LLONG_MIN, LLONG_MAX);
    return bestAction.move;
}

bool GomokuAgent::agentWon() {
}

bool GomokuAgent::opponentWon() {
}

bool GomokuAgent::hasStoneNearby(int x, int y, Board &state) {
    int delta[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, +1},
                       {+1, +1}, {+1, 0}, {+1, -1}, {0, -1}};
    for (int i = 0; i < 8; i++) {
        int xNext = x + delta[i][0];
        int yNext = y + delta[i][1];
        if (isOutOfBound(xNext, yNext))
            continue;
        if (state[xNext][yNext] != '.')
            return true;
    }
    return false;
}

GomokuAgent::Action GomokuAgent::maxValue(Board &state, int depth, long long alpha, long long beta) {
    //printBoard();
    if (depth == 0) {
        Action action(Move(), evaluate(state, agentCharacter));
        return action;
    }
    Move bestMove;
    long long minimax = LLONG_MIN;
    unordered_set<Move, MoveKeyHash, MoveKeyEqual>::iterator itr = remainingMoveList.begin();
    for (; itr != remainingMoveList.end(); itr++) {
        Move move = *itr;
        if (state[move.x][move.y] == '.' && hasStoneNearby(move.x, move.y, state)) {
            state[move.x][move.y] = agentCharacter;
            if (hasUnbrokenChainOfLen(winningChainLength, move, agentCharacter, state)
                || hasOpen(winningChainLength - 1, agentCharacter, state)) {
                minimax = LLONG_MAX;
                bestMove = move;
                state[move.x][move.y] = '.';
                break;
            }
            Action action = minValue(state, depth - 1, alpha, beta);
            if (action.minimax > minimax) {
                minimax = action.minimax;
                bestMove = move;
            }
            if (minimax >= beta) {
                state[move.x][move.y] = '.';
                break;
            }
            alpha = max(alpha, minimax);
            state[move.x][move.y] = '.';
        }
    }
    return Action(bestMove, minimax);
}

GomokuAgent::Action GomokuAgent::minValue(Board &state, int depth, long long alpha, long long beta) {
    //printBoard();
    if (depth == 0) {
        Action action(Move(), evaluate(state, opponentCharacter));
        return action;
    }
    Move bestMove;
    long long minimax = LLONG_MAX;
    unordered_set<Move, MoveKeyHash, MoveKeyEqual>::iterator itr = remainingMoveList.begin();
    for (; itr != remainingMoveList.end(); itr++) {
        Move move = *itr;
        if (state[move.x][move.y] == '.' && hasStoneNearby(move.x, move.y, state)) {
            state[move.x][move.y] = opponentCharacter;
            if (hasUnbrokenChainOfLen(winningChainLength, move, opponentCharacter, state)
                || hasOpen(winningChainLength - 1, opponentCharacter, state)) {
                minimax = LLONG_MIN;
                bestMove = move;
                state[move.x][move.y] = '.';
                break;
            }
            Action action = maxValue(state, depth - 1, alpha, beta);
            if (action.minimax < minimax) {
                minimax = action.minimax;
                bestMove = move;
            }
            if (minimax <= alpha) {
                state[move.x][move.y] = '.';
                break;
            }
            beta = min(beta, minimax);
            state[move.x][move.y] = '.';
        }
    }
    return Action(bestMove, minimax);
}

long long GomokuAgent::evaluate(Board &state, char nextTurnChar) {
    if (nextTurnChar == agentCharacter) {
        if (hasOpen(winningChainLength - 1, agentCharacter, state) || hasCapped(winningChainLength - 1, agentCharacter, state))
            return LLONG_MAX;
    }
    if (nextTurnChar == opponentCharacter) {
        if (hasOpen(winningChainLength - 1, opponentCharacter, state) || hasCapped(winningChainLength - 1, opponentCharacter, state))
            return LLONG_MIN;
    }
    long long utility = 0;
    for (int i = winningChainLength - 1; i >= 1; i--) {
        if (hasOpen(i, agentCharacter, state))
            utility += pow(2, i);
        if (hasCapped(i, agentCharacter, state))
            utility += pow(2, i) / 2;
        if (hasOpen(i, opponentCharacter, state))
            utility -= pow(2, i) / 2;
        if (hasCapped(i, opponentCharacter, state))
            utility -= pow(2, i) / 2 / 2;
    }
    return utility;
}

bool GomokuAgent::hasOpenStartingFrom(int x, int y, int dir, int len, char pieceType, Board &state) {
    if (state[x][y] != pieceType)
        return false;
    int xNext = x - delta[dir][0];
    int yNext = y - delta[dir][1];
    if (isOutOfBound(xNext, yNext))
        return false;
    if (state[xNext][yNext] != '.')
        return false;
    xNext = x;
    yNext = y;
    int count = 0;
    while (true) {
        if (isOutOfBound(xNext, yNext))
            return false;
        if (state[xNext][yNext] != pieceType)
            return false;
        count++;
        if (count == len)
            break;
        xNext += delta[dir][0];
        yNext += delta[dir][1];
    }
    if (count != len)
        return false;
    xNext += delta[dir][0];
    yNext += delta[dir][1];
    if (isOutOfBound(xNext, yNext))
        return false;
    if (state[xNext][yNext] == '.')
        return true;
}

bool GomokuAgent::hasOpen(int len, char pieceType, Board &state) {
    for (int i = 0; i < boardDimension; i++)
        for (int j = 0; j < boardDimension; j++)
            for (int k = 0; k < 4; k++) {
                if (hasOpenStartingFrom(i, j, k, len, pieceType, state))
                    return true;
            }
    return false;
}

bool GomokuAgent::hasCappedStartingFrom(int x, int y, int dir, int len, char pieceType, Board &state) {
    int cappedCount = 0;
    if (state[x][y] != pieceType)
        return false;
    int xNext = x - delta[dir][0];
    int yNext = y - delta[dir][1];
    if (isOutOfBound(xNext, yNext))
        cappedCount++;
    else {
        if (state[xNext][yNext] != pieceType && state[xNext][yNext] != '.')
            cappedCount++;
    }
    xNext = x;
    yNext = y;
    int count = 0;
    while (true) {
        if (isOutOfBound(xNext, yNext))
            return false;
        if (state[xNext][yNext] != pieceType)
            return false;
        count++;
        if (count == len)
            break;
        xNext += delta[dir][0];
        yNext += delta[dir][1];
    }
    if (count != len)
        return false;
    xNext += delta[dir][0];
    yNext += delta[dir][1];
    if (isOutOfBound(xNext, yNext))
        cappedCount++;
    else {
        if (state[xNext][yNext] != pieceType && state[xNext][yNext] != '.')
            cappedCount++;
    }
    return (cappedCount == 1);
}

bool GomokuAgent::hasCapped(int len, char pieceType, Board &state) {
    for (int i = 0; i < boardDimension; i++)
        for (int j = 0; j < boardDimension; j++)
            for (int k = 0; k < 4; k++) {
                if (hasCappedStartingFrom(i, j, k, len, pieceType, state))
                    return true;
            }
    return false;
}
