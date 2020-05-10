#include "Pool.h"

Pool::Pool(const std::vector <char>& letters)
{
    allLetters = letters;
}
char Pool::draw()
{
    if (allLetters.size() != 0)
    {
        int number = rand() % (allLetters.size());
        char c = allLetters.at(number);
        allLetters.erase(allLetters.begin() + number);
        return c;
    }
    else
        return ' ';
}
