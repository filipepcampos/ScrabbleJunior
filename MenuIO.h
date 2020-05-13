#ifndef MENUIO_H
#define MENUIO_H

#include <string>
#include <vector>
#include "Player.h"

namespace menuIO{
    /**
     * Display sorted scores of each player
     * @param players
     */
    void displayScores(std::vector<Player> players);

    /**
     * Read number of players
     * @return number of players (from 2 to 4) or 0 if EOF has occurred
     */
    int readNumPlayers();

    /**
     * Read menu option
     * @return option chosen or '2' if EOF has occurred (causing exit)
     */
    char readMenu();

    void showInstructions();

    /**
     * Read name of board
     * @return board name or empty string if EOF has occurred
     */
    std::string readBoardName();

    /**
     * Stall game waiting for input before returning to menu
     */
    void returnToMenu();

    /**
     * Output invalid board or player amount message (If a board has 14 letters there can't be more than 2 players for example)
     */
    void invalid();
}
#endif
