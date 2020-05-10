#include "Pool.h"

#ifndef LETTERSBAG_H
#define LETTERSBAG_H

class LettersBag
{
private:
    Pool* m_pool;

public:
    std::vector<char>lettersBag;
    void addRandomLetter();
    explicit LettersBag(Pool& pool);
    void showBag();
    void removeLetter(char letter);

};
#endif
