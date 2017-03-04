#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <vector>

using namespace std;

class Player {

public:
    Board *board;

    int empty_spaces;
    std::vector<Move> * adjacent;
    std::vector<Move> * occupied;

    Side side;
    Side opponentSide;

    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *calculate_move(int ms_for_move);
    void makeMoveOnBoard(Move * move, Side side);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
