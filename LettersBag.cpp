#include "LettersBag.h"
#include <algorithm>
#include <iostream>

LettersBag::LettersBag(Pool &pool) : m_pool(&pool) {
    for (int i = 0; i < 7; i++)
    {
        char c = m_pool->draw();
        lettersBag.push_back(c);
    }
    std::sort(lettersBag.begin(), lettersBag.end());
}

void LettersBag::showBag() const{
    for(auto c : lettersBag){
        std::cout << c << ", ";
    }
    std::cout << std::endl;
}

int LettersBag::addRandomLetter(int n) {
    int letters_drawn = 0;
    for(int i = 0; i < n; i++){
        char c = m_pool->draw();
        if(c){
            letters_drawn++;
            lettersBag.push_back(c);
        }
    }
    std::sort(lettersBag.begin(), lettersBag.end());
    return letters_drawn;
}

void LettersBag::remove(char c) {
    auto pos = std::find(lettersBag.begin(), lettersBag.end(), c);
    if(pos != lettersBag.end()){
        lettersBag.erase(pos);
    }
}

void LettersBag::exchangeTile(char c) {
    if(m_pool->canDraw()){
        addRandomLetter();
        remove(c);
        m_pool->add(c);
    }
}

int LettersBag::getSize() const{
    return lettersBag.size();
}

bool LettersBag::find(char c, const std::vector<char> &vec) const{
    return std::find(vec.begin(), vec.end(), c) != vec.end();
}
bool LettersBag::find(char c) const{
    return std::find(lettersBag.begin(), lettersBag.end(), c) != lettersBag.end();
}

bool LettersBag::intersects(const std::vector<char> &vec) const{
    for(const auto &c : lettersBag){
        if(find(c, vec)){
            return true;
        }
    }
    return false;
}
