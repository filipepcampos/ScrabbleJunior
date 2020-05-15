#ifndef MENUIO_H
#define MENUIO_H

#include <string>
#include <vector>
#include "Player.h"

namespace MenuIO{
    /**
     * Display sorted scores of each player
     * @param players
     */
    void displayScores(const std::vector<Player> &players);

    void returnToMenu();

    /**
     * Read number of players
     * @return number of players (from 2 to 4) or 0 if EOF has occurred
     */
    int readNumPlayers();
    bool testNumPlayers(const int &n);

    /**
     * Read menu option
     * @return option chosen or '2' if EOF has occurred (causing exit)
     */
    int readMenu();
    bool testMenu(const int &n);

    void showInstructions();

    /**
     * Read name of board
     * @return board name or empty string if EOF has occurred
     */
    std::string readBoardName();
    bool testBoardName(const std::string &s);


    /**
     * Output invalid board or player amount message (If a board has 14 letters there can't be more than 2 players for example)
     */
    void invalid();

};
#endif
