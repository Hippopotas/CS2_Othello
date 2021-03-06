#include <iostream>
#include "common.hpp"
#include "player.hpp"
#include "board.hpp"

// Use this file to test your minimax implementation (2-ply depth, with a
// heuristic of the difference in number of pieces).
int main(int argc, char *argv[]) {

    // Create board with example state. You do not necessarily need to use
    // this, but it's provided for convenience.
    char boardData[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', 'b', ' ', ' ', ' ', ' ', ' ', ' ',
        'b', 'w', 'b', 'b', 'b', 'b', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    };
    Board *board = new Board();
    board->setBoard(boardData);

    // Initialize player as the white player, and set testing_minimax flag.
    Player *player = new Player(WHITE);
    player->testingMinimax = true;

    delete player->board;
    player->board = board;

    while (player->adjacent->size() > 0) {
        player->adjacent->pop_back();
    }
    player->adjacent->push_back(Move(0, 4));
    player->adjacent->push_back(Move(1, 4));
    player->adjacent->push_back(Move(2, 4));
    player->adjacent->push_back(Move(3, 4));
    player->adjacent->push_back(Move(4, 4));
    player->adjacent->push_back(Move(5, 4));
    player->adjacent->push_back(Move(6, 4));
    player->adjacent->push_back(Move(6, 3));
    player->adjacent->push_back(Move(6, 2));
    player->adjacent->push_back(Move(5, 2));
    player->adjacent->push_back(Move(4, 2));
    player->adjacent->push_back(Move(3, 2));
    player->adjacent->push_back(Move(2, 2));
    player->adjacent->push_back(Move(2, 1));
    player->adjacent->push_back(Move(1, 1));
    player->adjacent->push_back(Move(0, 1));
    player->adjacent->push_back(Move(0, 2));
    
    while (player->occupied->size() > 0) {
        player->occupied->pop_back();
    }
    player->occupied->push_back(Move(0, 3));
    player->occupied->push_back(Move(1, 3));
    player->occupied->push_back(Move(1, 2));
    player->occupied->push_back(Move(2, 3));
    player->occupied->push_back(Move(3, 3));
    player->occupied->push_back(Move(4, 3));
    player->occupied->push_back(Move(5, 3));

    // Get player's move and check if it's right.
    Move *move = player->doMove(nullptr, -1);

    if (move != nullptr && move->x == 1 && move->y == 1) {
        std::cout << "Correct move: (1, 1)" << std::endl;;
    } else {
        std::cout << "Wrong move: got ";
        if (move == nullptr) {
            std::cout << "PASS";
        } else {
            std::cout << "(" << move->x << ", " << move->y << ")";
        }
        std::cout << ", expected (1, 1)" << std::endl;
    }

    delete move;
    delete player;

    return 0;
}
