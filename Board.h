#pragma once
#include <string>
#include <map>
#include <vector>

#define COLOR "\u001b[31m"
#define RESET "\u001b[0m"


/**
 * Word to be placed on the board
 * vertical_char and horizontal_char represent input ('A' through 'Z' and 'a' through 'z')
 */
struct Word{
    char vertical_char;
    char horizontal_char;
    char orientation;
    std::string text;
};

enum orientation{H, V};

/**
 * Mark the start and end of a word if a tile is placed on a marker it is shifted to the nearest tile that has not been
 * placed.
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
 * Each position includes information for horizontal and vertical line
 */
struct Position{
    char letter = ' ';
    bool placed = false;
    std::map<orientation, Markers> markers = {{H, {}}, {V, {}}};
};

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
    Position **m_board;
    int m_height = 0, m_width = 0;
    int m_empty_tiles = 0;

    /**
     * Try to place a letter on the board
     * Verifies if a letter can be placed returning the bool result
     * If a word was completed points will be added through reference
     * @param vertical_pos
     * @param horizontal_pos
     * @param &points
     * @return (bool) valid or not
     */
    bool placeLetter(char letter, int vertical_pos, int horizontal_pos, int &points);

    /**
     * Add a Word to m_board and m_board_info
     * @param word
     * @return (none)
     */
    void addWord(Word &word);

    /**
     * Shift start and end markers from starting position along line and direction
     * If direction == -1, end marker will be shifted to the left
     * If direction == 1, start marker will be shifted to the right
     * @param v
     * @param h
     * @param line
     * @param direction
     */
    void shiftMarker(int v, int h, orientation line, int direction);

    /**
     * Read Words from board file and fill the board
     * @return (none)
     */
    void fillBoard(std::ifstream &file);
};