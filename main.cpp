#include <iostream>
#include "MenuIO.h"
#include "Board.h"
#include "Player.h"
#include "Pool.h"
#include "Colors.h"
#include "Utility.h"

void runGame(const std::string &file_name, int num_players){
    Board board(file_name);
    Pool pool(board.getLetters());

    if(board.valid(num_players)){
        std::vector<Player> players;
        players.reserve(num_players);
        for(int i = 0; i < num_players; i++){
            players.emplace_back(board, pool);
        }
        int i = 0;
        while(!board.gameOver() && !std::cin.eof()){
            players[i].play();
            i = ++i % num_players;
        }
        MenuIO::displayScores(players);
    }
    else{
        MenuIO::invalid();
    }
}

int main() {
    while(!std::cin.eof()){
        int option, num_players;
        std::string board_name;
        try{
            option = MenuIO::readMenu();
            if(option == 3){
                break;
            }
            board_name = MenuIO::readBoardName();
            num_players = MenuIO::readNumPlayers();
        }
        catch(const Utility::CinEofError &e){
            break; // If EOF happens, program closes normally
        }
        runGame(board_name, num_players);
        MenuIO::returnToMenu();
    }
    std::cout << std::endl;
    return 0;
}
