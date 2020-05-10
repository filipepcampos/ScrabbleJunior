#include "Player.h"


Player::Player(Board& board, Pool& pool)
{
    LettersBag Bag(pool);
    m_bag = &Bag;
    m_board = &board;
    score = 0;
}
void Player::play()
{
    readInput();
    score += m_board->play(letter, vertical_char, horizontal_char);
}
void Player::readInput()
{
    (*m_bag).showBag;
    char delimiter;
    std::cout << "Choose one letter(X) from your bag and the position(Aa) you want to place them\n(\"X-Aa\") :  ";
    while (!(std::cin >> letter >> delimiter >> vertical_char >> horizontal_char) || !(isupper(vertical_char)) || !(islower(horizontal_char)) || delimiter != '-' ||
        (std::count(m_bag->lettersBag.begin(), m_bag->lettersBag.end(), vertical_char) == 0) || (std::count(m_bag->lettersBag.begin(), m_bag->lettersBag.end(), horizontal_char) == 0))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, please re-enter." << std::endl;

    }
}
