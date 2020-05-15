#include "Board.h"
#include <fstream>
#include <limits>
#include <iostream>
#include <cassert>
#include "Colors.h"

Board::Board(const std::string &file_name){
    std::ifstream file;
    std::string complete_file_name = file_name + ".txt";
    file.open(complete_file_name);
    if(!file.is_open()){
        m_valid = false;
        return;
    }

    char delimiter{};
    file >> m_height >> delimiter >> m_width;
    if(file.fail() || delimiter != 'x' || m_height <= 0 || m_height > 20 || m_width <= 0 || m_width > 20){
        m_valid = false;
        return;
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    m_board = new detail::Position* [m_height];
    for(int i = 0; i < m_height; ++i){
        m_board[i] = new detail::Position[m_width];
    }
    fillBoard(file);
    m_empty_tiles = m_total_tiles;
    file.close();
}

Board::~Board(){
    if(m_board){
        for(int i=0; i < m_height; ++i){
            delete [] m_board[i];
        }
        delete [] m_board;
    }
}

void Board::fillBoard(std::ifstream &file) {
    std::string buffer;
    while(getline(file, buffer) && !buffer.empty()){
        char v_char = buffer[0], h_char = buffer[1], orientation = buffer[3];
        std::string text = buffer.substr(5);
        int v_pos = v_char - 'A', h_pos = h_char - 'a';
        detail::Word w{v_pos, h_pos, orientation, text};
        if(!addWord(w)){
            m_valid = false;
            break;
        }
    }
}

bool Board::addWord(const detail::Word &word) {
    if(validateWord(word)){
        detail::orientation line = word.orientation == 'V' ? detail::V : detail::H;
        for (int i = 0; i < word.text.length(); ++i) {
            detail::Position *p = getPosition(word.vertical_pos, word.horizontal_pos, line, i);
            if(p->letter == ' '){
                m_total_tiles++;
                p->letter = word.text[i];
            }
            else if(p->letter != word.text[i]){
                return false;
            }
        }

        m_board[word.vertical_pos][word.horizontal_pos].markers[line].start = true;
        int offset = word.text.length() - 1;
        getPosition(word.vertical_pos, word.horizontal_pos, line, offset)->markers[line].end = true;
        return true;
    }
    return false;
}

bool Board::validateWord(const detail::Word &word) const{
    for(auto c : word.text){
        if(!isalpha(c)){
            return false;
        }
    }
    int orientation_int = word.orientation == 'V' ? 1 : 0;
    // orientation_int is used to move along word's line
    return !(word.horizontal_pos < 0  || word.horizontal_pos + word.text.length() * (1 - orientation_int) > m_width
             || word.vertical_pos < 0 || word.vertical_pos + word.text.length() * orientation_int > m_height
             || (word.orientation != 'H' && word.orientation != 'V'));
}

int Board::play(char letter, char vertical_char, char horizontal_char){
    int v_pos = vertical_char - 'A', h_pos = horizontal_char - 'a';
    if (validateLetter(letter, v_pos, h_pos)){
        return placeLetter(v_pos, h_pos);
    }
    return -1;
}

bool Board::validateLetter(char letter, int v_pos, int h_pos) const{
    if (v_pos < 0 || v_pos >= m_height || h_pos < 0 || h_pos >= m_width
        || m_board[v_pos][h_pos].placed   || m_board[v_pos][h_pos].letter != letter){
        return false;
    }
    bool valid[2] = {false, false};
    detail::orientation lines[2] = {detail::H, detail::V};

    for(int i = 0; i <= 1; i++){
        // Letter is valid if it coincides with start marker (which marks first playable letter of a word)
        detail::orientation line = lines[i];
        valid[line] = m_board[v_pos][h_pos].markers[line].start;
    }
    return valid[0] || valid[1];
}

int Board::placeLetter(int v_pos, int h_pos){
    m_board[v_pos][h_pos].placed = true;
    m_empty_tiles--;
    int points = 0;
    detail::orientation lines[] = {detail::H, detail::V};

    for(int i = 0; i <= 1; i++){
        detail::orientation line = lines[i];
        bool placed_behind = m_board[v_pos][h_pos].markers[line].start;
        bool placed_front = m_board[v_pos][h_pos].markers[line].end;

        m_board[v_pos][h_pos].markers[line].start = false;
        m_board[v_pos][h_pos].markers[line].end = false;

        if(placed_behind && placed_front){ // Word was completed
            points++;
        }
        else{
            // If word wasn't completed yet, shift the markers
            if(!placed_behind && placed_front){
                shiftMarker(v_pos, h_pos, line, -1); // Shift end marker toward start marker
            }
            if(!placed_front && placed_behind) {
                shiftMarker(v_pos, h_pos, line, 1); // Shift start marker toward end marker
            }
        }
    }
    return points;
}

void Board::shiftMarker(int v, int h, detail::orientation line, int direction){
    assert(direction == 1 || direction == -1);
    detail::Position *pos;
    short i = 1;
    do{
        pos = getPosition(v, h, line, i++ * direction);
    } while(pos->placed);

    if(direction == 1){
        pos->markers[line].start = true;
    }
    else{
        pos->markers[line].end = true;
    }
}

detail::Position *Board::getPosition(int v, int h, detail::orientation line, int n) {
    return &m_board[v + n * line][h + n * (1 - line)];
}

std::vector<char> Board::getLetters() const{
    std::vector<char> letters;
    letters.reserve(m_total_tiles);
    for(int i = 0; i < m_height; i++){
        for(int j = 0; j < m_width; j++){
            if(m_board[i][j].letter != ' '){
                letters.push_back(m_board[i][j].letter);
            }
        }
    }
    return letters;
}

std::vector<char> Board::getPlayableLetters() const{
    std::vector<char> letters;
    for(int i = 0; i < m_height; i++){
        for(int j = 0; j < m_width; j++){
            if(m_board[i][j].markers[detail::H].start || m_board[i][j].markers[detail::V].start){
                letters.push_back(m_board[i][j].letter);
            }
        }
    }
    return letters;
}

bool Board::gameOver() const{
    return m_empty_tiles <= 0;
}

bool Board::valid(int n) const {
    return m_valid && m_empty_tiles >= 7 * n;
}

void Board::print() const{
    std::cout << CLEAR;
    // Print top line
    std::cout << "\n  │ ";
    for(char c = 'a'; c < 'a' + m_width; ++c){
        std::cout << c << " ";
    }
    std::cout << "\n──┴";

    for(int i = 0; i < 2 * m_width; ++i){
        std::cout << "─";
    }
    std::cout << '\n';

    // Print board
    for(int i = 0; i < m_height; ++i){
        char row_char = 'A' + i;
        std::cout << row_char << " │ ";
        for(int j = 0; j < m_width; ++j){
            std::string color_code = m_board[i][j].placed ? RED : "";
            bool start_of_word = (m_board[i][j].markers[0].start || m_board[i][j].markers[1].start);
            color_code = start_of_word ? GREEN : color_code;
            std::cout << color_code << m_board[i][j].letter << ' ' << RESET;
        }
        std::cout << '\n';
    }
}
