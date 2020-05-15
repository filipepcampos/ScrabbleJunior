#ifndef PLAYER_H
#define PLAYER_H
#include "Pool.h"
#include "Board.h"
#include "LettersBag.h"

namespace detail{
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
    void play();

private:
    Board* const m_board;
    Pool* const m_pool;
    LettersBag lettersBag;
    static int id_counter;
    const int id;
    int score;

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

    /**
     * Read turn input
     * @return true if read was successful, false otherwise (EOF)
     */
    bool readPlay(detail::Play &play) const;
    bool testPlay(const detail::Play &play, const std::vector<char> &playable_letters) const;
    static bool convertToPlay(detail::Play &play, const std::string &str);

    /**
     * Read a single letter to be exchanged
     * @return char present in letterBag or 0 if EOF has occurred
     */
    char readLetterToExchange() const;
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