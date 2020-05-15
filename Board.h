#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <map>
#include <vector>

/**
 * This namespace is used to keep helper-structs that are not meant to be used outside this class
 */
namespace detail{
    /**
     * Word to be placed on the board
     * vertical_char and horizontal_char represent input ('A' through 'Z' and 'a' through 'z')
     * orientation is a char 'H' (horizontal) or 'V' (vertical)
     */
        struct Word{
            int vertical_pos;
            int horizontal_pos;
            char orientation;
            std::string text;
        };

    /**
     * Represents a line orientation (Horizontal or Vertical)
     */
        enum orientation{H = 0, V};

    /**
     * Mark the start and end of a word if a tile is placed on a marker it is shifted to the nearest unplaced position
     * If start = true the position is playable and the first non-empty tile of a word along a specific line
     * If end = true the position if the last playable tile of a word
     */
        struct Markers{
            bool start = false;
            bool end = false;
        };

    /**
     * Represent a position in the game board
     * If a letter has been placed in this position, placed = true
     * Each position includes information (markers) for horizontal and vertical line, the
     * usually the array will be accessed using orientation as index
     */
        struct Position{
            char letter = ' ';
            bool placed = false;
            Markers markers[2]{};
        };
}

class Board{
public:
    explicit Board(const std::string &file_name);
    ~Board();

    /**
     * Place a tile on a certain position of the board if valid
     * @param letter (char)
     * @param vertical_char (char) from 'A' to 'Z' (if within board range)
     * @param horizontal_char (char) from 'a' to 'z' (if within board range)
     * @return (int) points gained from the play
     */
    int play(char letter, char vertical_char, char horizontal_char);

    /**
     * Print the game board to the screen
     * @return (none)
     */
    void print() const;

    /**
     * Check if game has ended
     * @return (bool) true if game is over, false otherwise
     */
     bool gameOver() const;

     /**
      * Return if board is valid and large enough for n players
      * This condition should be checked before continuing to avoid any unwanted error
      * @param n - number of players
      * @return (bool)
      */
     bool valid(int n) const;

    /**
     * Get a std::vector<char> with all letters present in the board
     * @return (std::vector<char>)
     */
    std::vector<char> getLetters() const;

    /**
     * Get a std::vector<char> with all playable letters present in the board
     * @return (std::vector<char>)
     */
    std::vector<char> getPlayableLetters() const;

private:
    detail::Position **m_board = nullptr;
    int m_height = 0, m_width = 0;
    int m_total_tiles = 0;
    int m_empty_tiles = 0;
    bool m_valid = true;

    /**
     * Read Words from board file and fill the board
     * @return (none)
     */
    void fillBoard(std::ifstream &file);

    /**
     * Add a Word to m_board
     * @param word
     * @return (bool) true if word was added successfully, false otherwise
     */
    bool addWord(const detail::Word &word);

    /**
     * Verify if the given word is valid
     * @return (bool) true if valid, false otherwise
     */
    bool validateWord(const detail::Word &word) const;

    /**
     * Place a letter, shift markers if needed and return points won
     * @param vertical_pos
     * @param horizontal_pos
     * @return (int) points won by placement
     */
    int placeLetter(int vertical_pos, int horizontal_pos);

    /**
     * Verifies if a letter can be placed returning the bool result
     * @param vertical_pos
     * @param horizontal_pos
     * @return (bool) valid or not
     */
    bool validateLetter(char letter, int vertical_pos, int horizontal_pos) const;


    /**
     * Shift start or end marker from starting position along line and direction
     * If direction == -1, end marker will be shifted to the left
     * If direction == 1, start marker will be shifted to the right
     * Marker will be shifted to nearest unplaced position
     * @param v
     * @param h
     * @param line
     * @param direction
     */
    void shiftMarker(int v, int h, detail::orientation line, int direction);


    /**
     * Get nth position counting from (v, h) along the given line
     * @return (Position*)
     */
    detail::Position* getPosition(int v, int h, detail::orientation line, int n);
};

#endif