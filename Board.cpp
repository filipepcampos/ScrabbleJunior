#include "Board.h"
#include <fstream>
#include <string>
#include <limits>
#include <iostream>

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

std::vector<char> Board::getLetters(){
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

int Board::play(char letter, char vertical_char, char horizontal_char){
    int v_pos = vertical_char - 'A', h_pos = horizontal_char - 'a';
    if (validate(letter, v_pos, h_pos)){
        m_board[v_pos][h_pos].placed = true;
        return attributePoints(v_pos, h_pos);
    }
    return -1;
}

int Board::attributePoints(int v_pos, int h_pos) {
    int points = 0;
    orientation lines[2] = {H, V};
    for(int i = 0; i <= 1; i++){
        orientation line = lines[i];
        if(m_board[v_pos][h_pos].line[line]){
            bool complete = verifyTilePlacement(v_pos, h_pos, line, -1) && verifyTilePlacement(v_pos, h_pos, line, 1);
            points += complete;
        }
    }
    return points;
}

bool Board::validate(char letter, int v_pos, int h_pos){
    if (v_pos < 0 || v_pos > m_height - 1
        || h_pos < 0 || h_pos > m_width - 1
        || m_board[v_pos][h_pos].letter != letter){
        return false;
    }
    bool valid[2] = {false, false};

    orientation lines[2] = {H, V};
    for(int i = 0; i <= 1; i++){
        orientation line = lines[i];
        if(m_board[v_pos][h_pos].line[line]){
            valid[line] = verifyTilePlacement(v_pos, h_pos, line, -1);
        }
    }
    return valid[0] || valid[1];
}

void Board::print(){
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
        m_board[v_pos + i * line][h_pos + i * (1-line)].line[line] = true;
    }

    if(word.text.length() > 1){
        m_board[v_pos][h_pos].type[line] = 'S';
        int offset = word.text.length() - 1;
        m_board[v_pos + offset * line][h_pos + offset * (1-line)].type[line] = 'E';
    }
    else{
        m_board[v_pos][h_pos].type[line] = 'O';
    }

}

bool Board::verifyTilePlacement(int v_pos, int h_pos, orientation line, int direction) {
    bool complete = true;
    char target = direction == 1 ? 'E' : 'S';

    int n = -1;
    do{
        ++n;
        if(n != 0 && !m_board[v_pos + direction * n * line][h_pos + direction * n * (1-line)].placed) { // for n = 0 the tile will always be placed
            complete = false;
        }
    } while(m_board[v_pos + direction * n * line][h_pos + direction * n * (1-line)].type[line] != target &&
            m_board[v_pos + direction * n * line][h_pos + direction * n * (1-line)].type[line] != 'O');

    return complete;
}
