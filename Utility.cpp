#include "Utility.h"
#include <iostream>
#include <limits>

void Utility::pressToContinue(const std::string &msg) {
    if (!std::cin.eof()) {
        std::cout << msg << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}