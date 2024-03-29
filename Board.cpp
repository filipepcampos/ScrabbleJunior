#include "Board.h"
#include <fstream>
#include <string>
#include <limits>
#include <iostream>
#include <cassert>

Board::Board(const std::string &file_name){
    std::ifstream file;
    file.open(file_name);
    if(!file.is_open()){
        m_valid = false;
        return;
    }

    char delimiter;
    file >> m_height >> delimiter >> m_width;
    if(file.fail() || delimiter != 'x' || m_height <= 0 || m_height > 20 || m_width <= 0 || m_width > 20){
        m_valid = false;
        return;
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');


    m_board = new Position* [m_height];
    for(int i = 0; i < m_height; ++i){
        m_board[i] = new Position[m_width];
    }
    fillBoard(file);
    m_empty_tiles = m_total_tiles;
    file.close();
}

Board::~Board(){
    if(m_valid){
        for(int i=0; i < m_height; ++i){
            delete [] m_board[i];
        }
        delete [] m_board;
    }
}

void Board::fillBoard(std::ifstream &file) {
    std::string buffer;
    while(getline(file, buffer, '\n') && !buffer.empty() && !file.eof()){
        char v_char = buffer[0], h_char = buffer[1], orientation = buffer[3];
        std::string text = buffer.substr(5);
        int v_pos = v_char - 'A', h_pos = h_char - 'a';
        Word w{v_pos, h_pos, orientation, text};
        if(!addWord(w)){
            m_valid = false;
            break;
        }
    }
}

bool Board::addWord(const Word &word) {
    if(validateWord(word)){
        orientation line = word.orientation == 'V' ? V : H;
        for (int i = 0; i < word.text.length(); ++i) {
            Position *p = getPosition(word.vertical_pos, word.horizontal_pos, line, i);
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

bool Board::validateWord(const Word &word) const{
    if(word.orientation != 'H' && word.orientation != 'V'){
        return false;
    }
    for(auto c : word.text){
        if(!isalpha(c)){
            return false;
        }
    }
    int orientation_int = word.orientation == 'V' ? 1 : 0;
    return !(word.horizontal_pos < 0  || word.horizontal_pos + word.text.length() * (1 - orientation_int) > m_width
             || word.vertical_pos < 0 || word.vertical_pos + word.text.length() * orientation_int > m_height);
}

Position *Board::getPosition(int v, int h, orientation line, int n) {
    short line_int = line == V ? 1 : 0;
    return &m_board[v + n * line_int][h + n * (1 - line_int)];
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
            if(m_board[i][j].markers[H].start || m_board[i][j].markers[V].start){
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

int Board::play(char letter, char vertical_char, char horizontal_char){
    int v_pos = vertical_char - 'A', h_pos = horizontal_char - 'a';
    if (validateLetter(letter, v_pos, h_pos)){
        return placeLetter(v_pos, h_pos);
    }
    return -1;
}

bool Board::validateLetter(char letter, int v_pos, int h_pos) const{
    if (v_pos < 0 || v_pos > m_height - 1 || h_pos < 0 || h_pos > m_width - 1
                  || m_board[v_pos][h_pos].placed   || m_board[v_pos][h_pos].letter != letter){
        return false;
    }
    bool valid[2] = {false, false};
    orientation lines[2] = {H, V};

    for(int i = 0; i <= 1; i++){
        orientation line = lines[i];
        valid[line] = m_board[v_pos][h_pos].markers[line].start;
    }
    return valid[0] || valid[1];
}

int Board::placeLetter(int v_pos, int h_pos){
    m_board[v_pos][h_pos].placed = true;
    m_empty_tiles--;
    int points = 0;
    orientation lines[] = {H, V};

    for(int i = 0; i <= 1; i++){
        orientation line = lines[i];
        bool placed_behind = m_board[v_pos][h_pos].markers[line].start;
        bool placed_front = m_board[v_pos][h_pos].markers[line].end;

        m_board[v_pos][h_pos].markers[line].start = false;
        m_board[v_pos][h_pos].markers[line].end = false;

        if(placed_behind && placed_front){
            points++;
        }
        else{
            // If word wasn't completed yet, shift the markers
            if(!placed_behind && placed_front){
                shiftMarker(v_pos, h_pos, line, -1);
            }
            if(!placed_front && placed_behind) {
                shiftMarker(v_pos, h_pos, line, 1);
            }
        }
    }
    return points;
}

void Board::shiftMarker(int v, int h, orientation line, int direction){
    assert(direction == 1 || direction == -1);
    Position *pos;
    short i = 1;
    do{
        pos = getPosition(v, h, line, i * direction);
        i++;
    } while(pos->placed);

    if(direction == 1){
        pos->markers[line].start = true;
    }
    else{
        pos->markers[line].end = true;
    }
}

void Board::print() const{
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
            std::string color_code = m_board[i][j].placed ? COLOR : "";
            std::cout << color_code << m_board[i][j].letter << ' ' << RESET;
        }
        std::cout << '\n';
    }
}
