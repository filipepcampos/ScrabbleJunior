#ifndef PLAYER_H
#define PLAYER_H
#include "Pool.h"
#include "Board.h"
#include "LetterBag.h"

namespace detail{
    /**
     * Represents a play made by the player
     */
    struct Play{
        char letter;
        char vertical_char;
        char horizontal_char;
    };
}

class Player
{
public:
    Player(Board& board, Pool& pool);
    ~Player();
    int getScore() const;
    int getId() const;

    /**
     * Try to play two turns if possible
     * If no turn can be played, exchange two letters (if possible)
     */
    void play();

private:
    Board* const m_board;
    Pool* const m_pool;
    LetterBag letterBag;
    static int id_counter;
    const int id;
    int score;

    /**
     * Play a single turn
     * @return points won in turn
     */
    int playTurn();

    /**
     * Return if player can play a turn
     * @return
     */
    bool canPlay() const;

    /**
     * Read turn input
     * @return true if read was successful, false otherwise (EOF)
     */
    void readPlay(detail::Play &play) const;
    /**
     * Make sure Play is allowed
     * @param play
     * @param playable_letters
     * @return
     */
    bool testPlay(const detail::Play &play, const std::vector<char> &playable_letters) const;
    /**
     * Convert a std::string to Play
     * @param play
     * @param str
     * @return true if successful
     */
    static bool convertToPlay(detail::Play &play, const std::string &str);

    /**
     * Exchange two tiles (if not possible 1 or 0 tiles may be exchanged)
     * Tiles will be replaced on pool
     */
    void exchangeTiles();

    /**
     * Read a single letter to be exchanged
     * @return char present in letterBag or 0 if EOF has occurred
     */
    char readLetterToExchange() const;
    /**
     * Make sure letter is valid and in letterBag
     * @param c
     * @return
     */
    bool testLetterToExchange(const char &c) const;

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