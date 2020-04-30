#include "Board.h"
#include <fstream>
#include <string>
#include <limits>
#include <iostream>
#include <cassert>

Board::Board(const std::string &file_name){
    std::ifstream file;
    file.open(file_name);

    char delimiter;
    file >> m_height >> delimiter >> m_width;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    m_board = new Position* [m_height];
    for(int i = 0; i < m_height; ++i){
        m_board[i] = new Position[m_width];
    }

    fillBoard(file);
    file.close();
}

Board::~Board(){
    for(int i=0; i < m_height; ++i){
        delete [] m_board[i];
    }
    delete [] m_board;
}

std::vector<char> Board::getLetters() const{
    std::vector<char> letters;
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

int Board::play(char letter, char vertical_char, char horizontal_char){
    int v_pos = vertical_char - 'A', h_pos = horizontal_char - 'a';
    int points = 0;
    if (placeLetter(letter, v_pos, h_pos, points)){
        m_board[v_pos][h_pos].placed = true;
        return points;
    }
    return -1;
}

bool Board::placeLetter(char letter, int v_pos, int h_pos, int &points){
    if (v_pos < 0 || v_pos > m_height - 1 || h_pos < 0 || h_pos > m_width - 1
                  || m_board[v_pos][h_pos].placed   || m_board[v_pos][h_pos].letter != letter){
        return false;
    }
    bool valid[2] = {false, false};
    orientation lines[2] = {H, V};

    for(int i = 0; i <= 1; i++){
        orientation line = lines[i];
        // If placed_behind, all tiles behind the given position are placed
        // If placed_forward all tiles beyond the given position are placed
        bool placed_behind = m_board[v_pos][h_pos].markers[line].start;
        bool placed_front = m_board[v_pos][h_pos].markers[line].end;

        if(placed_behind || placed_front){
            valid[line] = placed_behind;
            points += placed_behind && placed_front;

            m_board[v_pos][h_pos].markers[line].start = false;
            m_board[v_pos][h_pos].markers[line].end = false;

            // If word wasn't completed yet, shift the markers
            if(!placed_behind){
                shiftMarker(v_pos, h_pos, line, -1);
            }
            if(!placed_front){
                shiftMarker(v_pos, h_pos, line, 1);
            }
        }
    }
    return valid[0] || valid[1];
}

void Board::shiftMarker(int v, int h, orientation line, int direction){
    assert(direction == 1 || direction == -1);
    do{
        v += line * direction;
        h += (1-line) * direction;
    } while(m_board[v][h].placed);

    if(direction == 1){
        m_board[v][h].markers[line].start = true;
    }
    else{
        m_board[v][h].markers[line].end = true;
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

void Board::fillBoard(std::ifstream &file) {
    std::string buffer;
    while(getline(file, buffer, '\n') && !buffer.empty()){
        char v_char = buffer[0], h_char = buffer[1], orientation = buffer[3];
        std::string text = buffer.substr(5);
        Word w{v_char, h_char, orientation, text};
        addWord(w);
    }
}

void Board::addWord(Word &word) {
    int v_pos = word.vertical_char - 'A', h_pos = word.horizontal_char - 'a';
    orientation line = word.orientation == 'V' ? V : H;

    for (int i = 0; i < word.text.length(); ++i) {
        m_board[v_pos + i * line][h_pos + i * (1-line)].letter = word.text[i];
    }

    m_board[v_pos][h_pos].markers[line].start = true;
    int offset = word.text.length() - 1;
    m_board[v_pos + offset * line][h_pos + offset * (1-line)].markers[line].end = true;
}