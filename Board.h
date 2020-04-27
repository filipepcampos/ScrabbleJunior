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


/**
 * Represent a position in the game board
 * If a letter has been placed in this position, placed = true
 * Orientation allows to distinguish between horizontal and vertical words (If someone plays a letter that only belongs
 * to a vertical word there's no need to check if a horizontal word was completed for example)
 * Type marks the start and end of a word with 'S' and 'E' for a given orientation (this allows to distinguish between
 * the start of a horizontal word from the start of a vertical word, for example). If a word only has a single letter
 * then it will be marked with 'O'
 */
struct Position{
    char letter = ' ';
    bool placed = false;
    std::map<char, bool> line = {{'H', false}, {'V', false}};
    std::map<char, char> type = {{'H', 0}, {'V', 0}};
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
    void print();

    /**
     * Get a std::vector<char> with all letters present in the board
     * @return
     */
    std::vector<char> getLetters();

private:
    Position **m_board;
    int m_height = 0, m_width = 0;

    /**
     * Make sure
     * @param vertical_pos
     * @param horizontal_pos
     * @return
     */
    bool validate(char letter, int vertical_pos, int horizontal_pos);

    /**
     * After making a play attribute the correct amount of points for words completed
     * @param v_pos
     * @param h_pos
     * @return
     */
    int attributePoints(int v_pos, int h_pos);

    /**
     * Verify if words is completed along a line in a certain direction
     * @param v_pos starting vertical position
     * @param h_pos starting horizontal position
     * @param line 'H' or 'V'
     * @param direction -1 or 1, if direction == -1 the method will check until 'S' is found, if
     *                 direction == 1, the method will check until 'E' is found
     * @return Bool, true if all tiles are placed along the given line and direction
     */
    bool verifyTilePlacement(int v_pos, int h_pos, char line, int direction);

    /**
     * Add a Word to m_board and m_board_info
     * @param word
     * @return (none)
     */
    void addWord(Word &word);

    /**
     * Read Words from board file and fill the board
     * @return (none)
     */
    void fillBoard(std::ifstream &file);
};