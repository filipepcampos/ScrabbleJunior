#include "MenuIO.h"
#include "Player.h"
#include "Colors.h"
#include "Utility.h"
#include <algorithm>

void MenuIO::displayScores(const std::vector<Player> &players){
    std::vector<std::pair<int, int>> scores;
    scores.reserve(players.size());
    for(const auto &p : players){
        scores.emplace_back(p.getId(), p.getScore());
    }
    std::stable_sort(scores.begin(), scores.end(), [](std::pair<int, int> p1, std::pair<int, int> p2)->bool{
                                                                            return p1.second > p2.second;
                                                                        });
    std::cout << CLEAR;
    if(scores[0].second > scores[1].second){
        int id = scores[0].first;
        std::cout << PLAYER_COLORS[id] << "Player " << id << RESET << " wins!\n\n";
    }
    else{
        std::cout << "Game ended in a tie\n\n";
    }
    std::cout << "SCOREBOARD\n";
    for(const auto &pair : scores){
        std::cout << PLAYER_COLORS[pair.first] << "Player " << pair.first << ": " << pair.second << RESET << std::endl;
    }
}

void MenuIO::returnToMenu() {
    Utility::pressToContinue("Press any key to return to menu...");
}

int MenuIO::readNumPlayers(){
    int num = 0;
    do{
        std::cout << CLEAR; std::cout << "Number of players (2 to 4): ";
    } while( !(Utility::read(num) && testNumPlayers(num)));
    return num;
}
bool MenuIO::testNumPlayers(const int &n) {
    return n >= 2 && n <= 4;
}

int MenuIO::readMenu() {
    int option;
    do{
        std::cout << CLEAR;
        std::cout << TITLE_COLOR << " _____                _     _     _          ___             _            \n"
                                    "/  ___|              | |   | |   | |        |_  |           (_)           \n"
                                    "\\ `--.  ___ _ __ __ _| |__ | |__ | | ___      | |_   _ _ __  _  ___  _ __ \n"
                                    " `--. \\/ __| '__/ _` | '_ \\| '_ \\| |/ _ \\     | | | | | '_ \\| |/ _ \\| '__|\n"
                                    "/\\__/ / (__| | | (_| | |_) | |_) | |  __/ /\\__/ / |_| | | | | | (_) | |   \n"
                                    "\\____/ \\___|_|  \\__,_|_.__/|_.__/|_|\\___| \\____/ \\__,_|_| |_|_|\\___/|_|\n\n"
                  << RESET;
        std::cout <<  "(1) - Start new game\n" << "(2) - How to play\n" <<"(3) - Exit game\n";
    } while( !(Utility::read(option) && testMenu(option)) );
    return option;
}
bool MenuIO::testMenu(const int &n){
    switch(n){
        case 1: case 3: return true;
        case 2: showInstructions();
    }
    return false;
}

void MenuIO::showInstructions() {
    std::cout << CLEAR;
    std::cout << TITLE_COLOR << " _   _                 _                _             \n"
                                "| | | |               | |              | |            \n"
                                "| |_| | _____      __ | |_ ___    _ __ | | __ _ _   _ \n"
                                "|  _  |/ _ \\ \\ /\\ / / | __/ _ \\  | '_ \\| |/ _` | | | |\n"
                                "| | | | (_) \\ V  V /  | || (_) | | |_) | | (_| | |_| |\n"
                                "\\_| |_/\\___/ \\_/\\_/    \\__\\___/  | .__/|_|\\__,_|\\__, |\n"
                                "                                 | |             __/ |\n"
                                "                                 |_|            |___/ \n\n" << RESET;
    std::cout << "On your turn, play two of your tiles by covering letters on the game board squares with matching tiles.\n" <<
    "You must cover the first available open letter of a word.\n" <<
    "Placed tiles will be presented in " << RED << "RED\n" << RESET <<
    "All first available open letters of a word will be " << GREEN << "GREEN\n" << RESET;
    Utility::pressToContinue();
}

std::string MenuIO::readBoardName(){
    std::string name;
    do{
        std::cout << CLEAR; std::cout << "Please enter board name";
    } while( !(Utility::read(name) && testBoardName(name)) );
    std::cout << std::endl;
    return name;
}
bool MenuIO::testBoardName(const std::string &name){
    for(const auto &c : name){
        if(!isalnum(c)){
            return false;
        }
    }
    return !name.empty(); // True if not empty
}


void MenuIO::invalid() {
    std::cout << ERROR_COLOR << "There are too many players or board isn't valid" << RESET << std::endl;
}
