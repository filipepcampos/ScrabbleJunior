#include "LettersBag.h"

LettersBag::LettersBag(Pool& pool)
{
    m_pool = &pool;
    for (int i = 0; i < 7; i++)
    {
        char c = m_pool->draw();
        lettersBag.push_back(c);
    }
}
void LettersBag::showBag()
{
    for (int i = 0; i < lettersBag.size(); i++) {
        std::cout << lettersBag[i] << ", ";
    }
    std::cout << std::endl;
}
void LettersBag::removeLetter(char letter)
{
    lettersBag.erase(std::find(lettersBag.begin(), lettersBag.end(), toupper(letter)));
}
void LettersBag::addRandomLetter()
{
    char c = m_pool->draw();
    lettersBag.push_back(c);
}