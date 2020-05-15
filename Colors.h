#ifndef COLORS_H
#define COLORS_H
#include <string>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

#define COLOR RED
#define TITLE_COLOR CYAN
#define RESET "\u001b[0m"
#define CLEAR "\033[2J\033[1;1H"
const std::string PLAYER_COLORS[4] = {MAGENTA, YELLOW, GREEN, BLUE};

#endif
