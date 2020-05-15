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

    /**
     * Wait for any user input before returning
     */
    void returnToMenu();

    /**
     * Read number of players
     * @return number of players (from 2 to 4)
     */
    int readNumPlayers();
    /**
     * Test if number of players is valid
     * @param n
     * @return
     */
    bool testNumPlayers(const int &n);

    /**
     * Read menu option
     * @return option chosen
     */
    int readMenu();
    /**
     * Test if option is valid and allowed to exit menu
     * Note: If option = 2 is NOT valid because intended behavior is return to menu,
     * instruction screen will be launched from here.
     * @param n
     * @return
     */
    bool testMenu(const int &n);

    /**
     * Show instructions screen
     */
    void showInstructions();

    /**
     * Read name of board
     * @return board name
     */
    std::string readBoardName();
    /**
     * Test if name is a valid board name
     * @param name
     * @return
     */
    bool testBoardName(const std::string &name);


    /**
     * Output invalid board or player amount message (If a board has 14 letters there can't be more than 2 players for example)
     */
    void invalid();
}
#endif
