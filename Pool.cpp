#include "Pool.h"
#include <algorithm>
#include <random>

Pool::Pool(const std::vector <char>& letters)
{
    allLetters = letters;
    shuffle(allLetters);
}

char Pool::draw()
{
    if (!allLetters.empty())
    {
        char c = allLetters.back();
        allLetters.pop_back();
        return c;
    }
    return 0;
}

void Pool::add(char c){
    allLetters.push_back(c);
    shuffle(allLetters);
}

void Pool::shuffle(std::vector<char> &vec){
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::shuffle(vec.begin(), vec.end(), mt);
}

int Pool::getSize() const{
    return allLetters.size();
}