#include <iostream>
#include "MenuIO.h"
#include "Board.h"
#include "Player.h"
#include "Pool.h"
#include "Colors.h"

int runGame(const std::string &file_name, int num_players){
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
        menuIO::displayScores(players);
    }
    else{
        menuIO::invalid();
    }
}

int main() {
    while(!std::cin.eof()){
        char option = menuIO::readMenu();
        if(option == '3'){break;}

        std::string boardName = menuIO::readBoardName();
        if(boardName.empty()){break;}

        int num_players = menuIO::readNumPlayers();
        if(!num_players){break;}

        runGame(boardName, num_players);
        menuIO::returnToMenu();
    }
    return 0;
}
