#include "player.hpp"
#include <tuple>
#include <math.h>
#include <cstring>

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

    values = new std::vector<int>();
    values->push_back(10);
    values->push_back(-1);
    values->push_back(-3);
    values->push_back(1);
    values->push_back(0);
    values->push_back(1);
    values->push_back(1);
    values->push_back(0);
    values->push_back(0);
    values->push_back(0);
}

/*
 * Returns assigned value of a board space (for simple heuristic function).
 */
int Player::value(int x, int y)
{
    int xr = x;
    int yr = y;
    
    if (x >= 4)
    {
        xr = 7 - x;
    }
    if (y >= 4)
    {
        yr = 7 - y;
    }
    
    if (yr > xr)
    {
        int temp = xr;
        xr = yr;
        yr = temp;
    }
    
    int index = (xr * (xr + 1)) / 2 + yr;

    return (*values)[index];
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete adjacent;
    delete occupied;
    delete board;
    delete values;
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
 * 
 */
int Player::calc_Heuristic(Board * board, Move * m, Side side)
{
    Board * temp_board = board->copy();
    temp_board->doMove(m, side);
    
    //a for after; b for before!
    int a = temp_board->count(side);
    int b = board->count(side);
    
    int score = a - b;

    score += value( m->getX(), m->getY());
    
    delete temp_board;
    
    return score;
}

std::vector<Move> * copyvec(std::vector<Move> * vec) {
    std::vector<Move> * ret = new std::vector<Move>();
    for (unsigned int i = 0; i < vec->size(); i++) {
        ret->push_back((*vec)[i]);
    }
    return ret;
}

/*
 * Minimax function
 */
std::tuple<Move*, int> Player::minimax(Board * board, std::vector<Move> * adjacent, 
                        std::vector<Move> * occupied, Side toMove, int layers)
{
    if (layers <= 1)
    {
        int value = -100;
        Move * mmax = nullptr;

        for (unsigned int i = 0; i < adjacent->size(); i++) {
            if (board->checkMove(&(*adjacent)[i], toMove)) {
                int x = (*adjacent)[i].getX();
                int y = (*adjacent)[i].getY();
                
                Move * move = new Move(x, y);
                int delta = calc_Heuristic(board, move, toMove);
                
                if (delta > value)
                {
                    value = delta;
                    delete mmax;
                    mmax = move;
                }
                else
                {
                    delete move;
                }

            }
        }
        
        if (value == -100) {
            value = 0;
        }
        return std::make_tuple(mmax, value);
    }
    else
    {
        Move * mmax = nullptr;
        int value = -100;
        
        for (unsigned int i = 0; i < adjacent->size(); i++) {
            if (board->checkMove(&(*adjacent)[i], side)) {
                int x = (*adjacent)[i].getX();
                int y = (*adjacent)[i].getY();
                
                Board * new_board = board->copy();
                std::vector<Move> * new_adj = copyvec(adjacent);
                std::vector<Move> * new_occ = copyvec(occupied);
                
                Move * moveToMake = new Move(x, y);
                makeMoveOnBoard(new_board, new_adj, new_occ, moveToMake, toMove, false);
                
                int before = board->count(toMove);
                int after = new_board->count(toMove);
                
                Side other = WHITE;
                if (toMove == WHITE) {
                    other = BLACK;
                }
                
                std::tuple<Move*, int> data = minimax(new_board, new_adj,
                                                new_occ, other, layers - 1);
                
                Move * move = std::get<0>(data);
                
                int delta = after - before - std::get<1>(data);
                
                if (delta > value)
                {
                    value = delta;
                    delete mmax;
                    mmax = moveToMake;
                }
                else
                {
                    delete moveToMake;
                }
                delete move;
                delete new_board;
                delete new_adj;
                delete new_occ;
            }
        }

        if (value == -100) {
            value = 0;
        }
        return std::make_tuple(mmax, value);
    }
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
    if (opponentsMove) {
        makeMoveOnBoard(board, adjacent, occupied, opponentsMove, opponentSide, true);
    }


    int moves_left = (empty_spaces / 2 + 1);
    int t_move = msLeft / moves_left;
    if (msLeft == -1) {
        t_move = -1;
    }

    Move * m = calculate_move(t_move);
    
    if (m) {
        makeMoveOnBoard(board, adjacent, occupied, m, side, true);
    }

    return m;
}

Move *Player::calculate_move(int msLeft) {
    if (msLeft == -1) {
        msLeft = 1001;
    }

    int layers = int(log10(msLeft) + 3);


    if (testingMinimax) {
        layers = 2;
    }

    layers = 3;
    
    //int begin = clock();
    Move * mmax = std::get<0>(minimax(board, adjacent, occupied, side, layers));
    //std::cerr << layers << " " << adjacent->size() << " " << clock() - begin << std::endl;

    return mmax;
}

void Player::makeMoveOnBoard(Board * board, std::vector<Move> * adjacent, 
                            std::vector<Move> * occupied, Move * m, Side side,
                            bool actual) {
    board->doMove(m, side);

    int x = m->x;
    int y = m->y;
    int adji = index_of(adjacent, x, y);
    if (adji >= 0) {
        (*adjacent)[adji] = (*adjacent)[adjacent->size() - 1];
        adjacent->pop_back();
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

    if (actual) {
        empty_spaces--;
    }
}
