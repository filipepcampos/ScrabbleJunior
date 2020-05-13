#ifndef PLAYER_H
#define PLAYER_H
#include "Pool.h"
#include "Board.h"
#include "LettersBag.h"

class Player
{
public:
    Player(Board& board, Pool& pool);
    ~Player();
    int getScore() const;
    int getId() const;
    void play();

private:
    Board* m_board;
    Pool* m_pool;
    LettersBag lettersBag;
    static int id_counter;
    const int id;
    int score;

    // ---------------- Data Methods ----------------
    /**
     * Play a single turn
     * @return points won in turn or -1 if EOF has occurred
     */
    int playTurn();

    /**
     * Return if player can play a turn
     * @return
     */
    bool canPlay() const;

    /**
     * Exchange two tiles (if not possible 1 or 0 tiles may be exchanged)
     * Tiles will be replaced on pool
     */
    void exchangeTiles();

    // ----------------- IO Methods ------------------
    /**
     * Read turn input
     * @param letter - letter to play
     * @param vertical_char - vertical position
     * @param horizontal_char - horizontal position
     * @return true if read was successful, false otherwise (EOF)
     */
    bool readInput(char &letter, char &vertical_char, char &horizontal_char) const;

    /**
     * Read a single letter to be exchanged
     * @return char present in letterBag or 0 if EOF has occurred
     */
    char readLetterToExchange();

    /**
     * Prompt user to press any key to continue
     */
    static void pressToContinue();

    /**
     * Skip a turn
     */
    void skipTurn() const;

    /**
     * Output player's score
     */
    void outputScore() const;

    /**
     * Output "invalid input" message
     */
    static void outputInvalid();
};
#endif