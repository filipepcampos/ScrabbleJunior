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
            if(m_board[i][j].info[H].start || m_board[i][j].info[V].end){
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
    if (v_pos < 0 || v_pos > m_height - 1
        || h_pos < 0 || h_pos > m_width - 1
        || m_board[v_pos][h_pos].placed
        || m_board[v_pos][h_pos].letter != letter){
        return false;
    }
    bool valid[2] = {false, false};

    orientation lines[2] = {H, V};
    for(int i = 0; i <= 1; i++){
        orientation line = lines[i];
        if(m_board[v_pos][h_pos].info[line].belongsToLine){
            // backwards and forwards show if all tiles behind and in front of the position are placed
            bool placed_behind = m_board[v_pos][h_pos].info[line].start;
            bool placed_front = checkForwardPlacement(v_pos, h_pos, line);
            valid[line] = placed_behind;
            points += placed_behind && placed_front;

            m_board[v_pos][h_pos].info[line].start = false;

            if(!placed_front && placed_behind){
                int v = v_pos, h = h_pos;
                do{
                    v += line;
                    h += 1-line;
                } while(m_board[v][h].placed);
                m_board[v][h].info[line].start = true;
            }
        }
    }
    return valid[0] || valid[1];
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
        Position *pos = getPosition(v_pos, h_pos, line, i);
        pos->letter = word.text[i];
        pos->info[line].belongsToLine = true;
    }

    m_board[v_pos][h_pos].info[line].start = true;
    int offset = word.text.length() - 1;
    getPosition(v_pos, h_pos, line, offset)->info[line].end = true;
}

Position* Board::getPosition(int v_pos, int h_pos, orientation line, int n){
    return &m_board[v_pos + n * line][h_pos + n * (1-line)];
}

bool Board::checkForwardPlacement(int v_pos, int h_pos, orientation line) {
    bool complete = true;
    int n = -1;
    Position *pos;
    do{
        ++n;
        pos = getPosition(v_pos, h_pos, line, n);
        if(n != 0 && !pos->placed) { // for n = 0 the tile will always be placed
            complete = false;
        }
    } while(!pos->info[line].end);

    return complete;
}
