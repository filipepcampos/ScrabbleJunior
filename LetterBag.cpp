#include "LetterBag.h"
#include <algorithm>
#include <iostream>

LetterBag::LetterBag(Pool &pool) : m_pool(&pool) {
    for (int i = 0; i < 7; i++)
    {
        char c = m_pool->draw();
        letterBag.push_back(c);
    }
    std::sort(letterBag.begin(), letterBag.end());
}

int LetterBag::addLetter(int n) {
    int letters_drawn = 0;
    for(int i = 0; i < n; i++){
        char c = m_pool->draw();
        if(c){
            letters_drawn++;
            letterBag.push_back(c);
        }
    }
    std::sort(letterBag.begin(), letterBag.end());
    return letters_drawn;
}

void LetterBag::showBag() const{
    for(const auto &c : letterBag){
        std::cout << c << ", ";
    }
    std::cout << std::endl;
}

void LetterBag::remove(char c) {
    auto pos = std::find(letterBag.begin(), letterBag.end(), c);
    if(pos != letterBag.end()){
        letterBag.erase(pos);
    }
}

void LetterBag::replaceTile(char c) {
    remove(c);
    m_pool->add(c);
}

int LetterBag::getSize() const{
    return letterBag.size();
}

bool LetterBag::find(char c, const std::vector<char> &vec) const{
    return std::find(vec.begin(), vec.end(), c) != vec.end();
}
bool LetterBag::find(char c) const{
    return std::find(letterBag.begin(), letterBag.end(), c) != letterBag.end();
}

bool LetterBag::intersects(const std::vector<char> &vec) const{
    for(const auto &c : letterBag){
        if(find(c, vec)){
            return true;
        }
    }
    return false;
}
