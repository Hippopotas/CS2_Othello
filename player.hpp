#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <vector>
#include <tuple>

using namespace std;

class Player {

public:
    Board *board;

    int empty_spaces;
    std::vector<Move> * adjacent;
    std::vector<Move> * occupied;
    std::vector<int> * values;
    bool storeValues;

    int value(int x, int y);
    
    int calc_Heuristic(Board * board, Move * m, std::vector<Move> *adjacent,
                        std::vector<Move> *occupied, Side side);
    std::tuple<Move*, int> minimax(Board * board, std::vector<Move> * adjacent, 
                        std::vector<Move> * occupied, Side toMove, int layers);

    Side side;
    Side opponentSide;

    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    Move *calculate_move(int ms_for_move);
    void makeMoveOnBoard(Board * board, std::vector<Move> * adj,
                        std::vector<Move> * occ, Move * move, Side side,
                        bool actual);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
