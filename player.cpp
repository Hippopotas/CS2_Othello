#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    board = new Board();
    empty_spaces = 60;


    this->side = side;
    if (side == WHITE) {
        opponentSide = BLACK;
    } else {
        opponentSide = WHITE;
    }

    adjacent = new std::vector<Move>();
    adjacent->push_back(Move(2, 2));
    adjacent->push_back(Move(2, 3));
    adjacent->push_back(Move(2, 4));
    adjacent->push_back(Move(2, 5));
    adjacent->push_back(Move(3, 5));
    adjacent->push_back(Move(4, 5));
    adjacent->push_back(Move(5, 5));
    adjacent->push_back(Move(5, 4));
    adjacent->push_back(Move(5, 3));
    adjacent->push_back(Move(5, 2));
    adjacent->push_back(Move(4, 2));
    adjacent->push_back(Move(3, 2));

    occupied = new std::vector<Move>();
    occupied->push_back(Move(3, 3));
    occupied->push_back(Move(3, 4));
    occupied->push_back(Move(4, 3));
    occupied->push_back(Move(4, 4));
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete adjacent;
    delete occupied;
    delete board;
}

int index_of(std::vector<Move> * moves, int x, int y) {
    for (unsigned int i = 0; i < moves->size(); i++) {
        if ((*moves)[i].x == x && (*moves)[i].y == y) {
            return int(i);
        }
    }
    return -1;
}







/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    if (opponentsMove != nullptr) {
        makeMoveOnBoard(opponentsMove, opponentSide);
    }

    int t_move = msLeft * 2 / empty_spaces;
    if (msLeft == -1) {
        t_move = -1;
    }

    Move * m = calculate_move(t_move);
    
    makeMoveOnBoard(m, side);
    
    return m;
}

Move *Player::calculate_move(int msLeft) {

    if (msLeft == -1) {
        msLeft = 60000;
    }
    for (unsigned int i = 0; i < adjacent->size(); i++) {
        if (board->checkMove(&(*adjacent)[i], side)) {
            int x = (*adjacent)[i].getX();
            int y = (*adjacent)[i].getY();
            Move * m = new Move(x, y);
            return m;
        }
    }
    return nullptr;
}

void Player::makeMoveOnBoard(Move * m, Side side) {
    board->doMove(m, side);

    int x = m->x;
    int y = m->y;
    int adji = index_of(adjacent, x, y);
    if (adji >= 0) {
        adjacent->erase(adjacent->begin() + adji);
    } else {
        std::cerr << "move not in adjacent (this should never occur)" << std::endl;
    }

    for (int dx = -1; dx < 2; dx++) {
        for (int dy = -1; dy < 2; dy++) {
            if (dx == 0 && dy == 0) {
                continue;
            }
            if (0 <= x + dx && x + dx < 8 && 0 <= y + dy && y + dy < 8) {
                if (index_of(occupied, x + dx, y + dy) == -1 &&
                        index_of(adjacent, x + dx, y + dy) == -1) {
                    adjacent->push_back(Move(x + dx, y + dy));
                }
            }
        }
    }
    occupied->push_back(Move(x, y));

    empty_spaces--;
}