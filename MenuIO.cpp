#include "MenuIO.h"
#include "Player.h"
#include "Colors.h"
#include "Utility.h"
#include <string>
#include <vector>
#include <algorithm>

void MenuIO::displayScores(const std::vector<Player> &players){
    std::vector<std::pair<int, int>> scores;
    scores.reserve(players.size());
    for(const auto &p : players){
        scores.emplace_back(p.getScore(), p.getId());
    }
    std::stable_sort(scores.begin(), scores.end(), [](std::pair<int, int> p1, std::pair<int, int> p2)->bool{
                                                                            return p1.first > p2.first;
                                                                        });
    std::cout << CLEAR;
    std::cout << TITLE_COLOR << "SCOREBOARD" << std::endl << std::endl;
    for(const auto &pair : scores){
        std::cout << PLAYER_COLORS[pair.second] << "Player " << pair.second << ": " << pair.first << RESET << std::endl;
    }
}

void MenuIO::returnToMenu() {
    Utility::pressToContinue("Press any key to return to menu...");
}

int MenuIO::readNumPlayers(){
    int num = 0;
    while(true){
        std::cout << CLEAR; std::cout << "Number of players (2 to 4): ";
        if(Utility::read(num) && (!num || testNumPlayers(num))){
            break;
        }
    }
    return num;
}
bool MenuIO::testNumPlayers(const int &n) {
    return n >= 2 && n <= 4;
}

int MenuIO::readMenu() {
    int option;
    while(true) {
        std::cout << CLEAR;
        std::cout << TITLE_COLOR << " _____                _     _     _          ___             _            \n"
                                    "/  ___|              | |   | |   | |        |_  |           (_)           \n"
                                    "\\ `--.  ___ _ __ __ _| |__ | |__ | | ___      | |_   _ _ __  _  ___  _ __ \n"
                                    " `--. \\/ __| '__/ _` | '_ \\| '_ \\| |/ _ \\     | | | | | '_ \\| |/ _ \\| '__|\n"
                                    "/\\__/ / (__| | | (_| | |_) | |_) | |  __/ /\\__/ / |_| | | | | | (_) | |   \n"
                                    "\\____/ \\___|_|  \\__,_|_.__/|_.__/|_|\\___| \\____/ \\__,_|_| |_|_|\\___/|_|\n\n"
                                    << RESET;
        std::cout <<  "(1) - Start new game\n" << "(2) - How to play\n" <<"(3) - Exit game\n\n";

        if(Utility::read(option) && (!option ||testMenu(option))){
            break;
        }
    }
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
    "Playable tiles will be " << GREEN << "GREEN\n" << RESET;
    Utility::pressToContinue();
}

std::string MenuIO::readBoardName(){
    std::string name{};
    while(true){
        std::cout << CLEAR; std::cout << "Please enter the board name";
        if(Utility::read(name) && (name.empty() || testBoardName(name))){
            break;
        }
    }
    std::cout << std::endl;
    return name;
}
bool MenuIO::testBoardName(const std::string &name){
    if(!name.empty()){
        for(const auto &c : name){
            if(!isalnum(c)){
                return false;
            }
        }
        return true;
    }
    return false;
}


void MenuIO::invalid() {
    std::cout << COLOR << "There are too many players or board isn't valid" << RESET << std::endl;
}
