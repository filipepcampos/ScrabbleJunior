#include "MenuIO.h"
#include "Player.h"
#include "Colors.h"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

void menuIO::displayScores(std::vector<Player> players){
    std::cout << CLEAR;
    std::vector<std::pair<int, int>> scores;
    scores.reserve(players.size());
    for(const auto &p : players){
        scores.emplace_back(p.getScore(), p.getId());
    }
    std::stable_sort(scores.begin(), scores.end(), [](std::pair<int, int> p1, std::pair<int, int> p2)->bool{
                                                                            return p1.first > p2.first;
                                                                        });
    std::cout << TITLE_COLOR << "SCOREBOARD" << std::endl << std::endl;
    for(const auto &pair : scores){
        std::cout << PLAYER_COLORS[pair.second] << "Player " << pair.second << ": " << pair.first << RESET << std::endl;
    }
}

int menuIO::readNumPlayers(){
    std::string buffer{};
    bool valid = false;
    int num = 0;
    while(!valid){
        std::cout << CLEAR; std::cout << "Number of players (2 to 4): ";
        valid = true;
        std::getline(std::cin, buffer);
        if(std::cin.eof()){
            std::cout << std::endl;
            return 0;
        }
        std::stringstream ss(buffer);
        ss >> num;
        if(ss.fail() || num < 2 || num > 4){
            valid = false;
        }
    }
    return num;
}

char menuIO::readMenu() {
    std::string buffer{};
    while (buffer != "1" && buffer != "3") {
        std::cout << CLEAR;
        std::cout << TITLE_COLOR << " _____                _     _     _          ___             _            \n"
                                    "/  ___|              | |   | |   | |        |_  |           (_)           \n"
                                    "\\ `--.  ___ _ __ __ _| |__ | |__ | | ___      | |_   _ _ __  _  ___  _ __ \n"
                                    " `--. \\/ __| '__/ _` | '_ \\| '_ \\| |/ _ \\     | | | | | '_ \\| |/ _ \\| '__|\n"
                                    "/\\__/ / (__| | | (_| | |_) | |_) | |  __/ /\\__/ / |_| | | | | | (_) | |   \n"
                                    "\\____/ \\___|_|  \\__,_|_.__/|_.__/|_|\\___| \\____/ \\__,_|_| |_|_|\\___/|_|\n\n"
                                    << RESET;
        std::cout <<  "(1) - Start new game\n" << "(2) - How to play\n" <<"(3) - Exit game\n\n" << "> ";
        std::getline(std::cin, buffer);

        if(buffer == "2"){
            showInstructions();
        }
        if (std::cin.eof()) {
            buffer = "3";
        }
    }
    std::cout << std::endl;
    return buffer[0];
}

void menuIO::showInstructions() {
    std::cout << CLEAR;
    std::cout << TITLE_COLOR << " _   _                 _                _             \n"
                                "| | | |               | |              | |            \n"
                                "| |_| | _____      __ | |_ ___    _ __ | | __ _ _   _ \n"
                                "|  _  |/ _ \\ \\ /\\ / / | __/ _ \\  | '_ \\| |/ _` | | | |\n"
                                "| | | | (_) \\ V  V /  | || (_) | | |_) | | (_| | |_| |\n"
                                "\\_| |_/\\___/ \\_/\\_/    \\__\\___/  | .__/|_|\\__,_|\\__, |\n"
                                "                                 | |             __/ |\n"
                                "                                 |_|            |___/ \n\n" << RESET;
    std::cout << "On your turn, play two of your tiles by covering letters on the gameboard squares with matching tiles.\n" <<
    "You must cover the first available open letter of a word.\n\n";
    returnToMenu();
}

std::string menuIO::readBoardName(){
    std::string buffer{};
    bool read = false;
    while(!read){
        read = true;
        std::cout << CLEAR; std::cout << "Please enter the board name\n" << "> ";
        std::getline(std::cin, buffer);
        if(std::cin.eof()){
            // If EOF occurs, board name has been "read" and empty str will be returned to signal EOF occurred
            buffer.clear();
        }
        else if(buffer.empty()){
            read = false;
        }
        for(const auto &c : buffer){
            if(!isalnum(c)){
                read = false;
            }
        }
    }
    std::cout << std::endl;
    return buffer;
}

void menuIO::returnToMenu() {
    if(!std::cin.eof()){
        std::cout << "Press any key to return to menu..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void menuIO::invalid() {
    std::cout << COLOR << "There are too many players or board isn't valid" << RESET << std::endl;
}
