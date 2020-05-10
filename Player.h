#include "Pool.h"
#include "LettersBag.h"
#include "Board.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
private:
    LettersBag* m_bag;
    Board* m_board;
    char letter, vertical_char, horizontal_char;
    int score;
    void readInput();
public:
    Player(Board& board, Pool& pool);
    void play();

};
#endif