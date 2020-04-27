#include <iostream>
#include "Board.h"

int main() {
    Board b("default.txt");
    b.print();
    std::cout << b.play('A', 'K', 'n') << std::endl;
    b.print();
    return 0;
}
