#ifndef LETTERSBAG_H
#define LETTERSBAG_H
#include <vector>
#include "Pool.h"

class LettersBag{
public:
    explicit LettersBag(Pool &m_pool);
    int addRandomLetter(int n = 1);
    void exchangeTile(char c);
    void showBag() const;
    int getSize() const;
    void remove(char c);
    bool find(char c, const std::vector<char> &vec) const;
    bool find(char c) const;
    bool intersects(const std::vector<char> &vec) const;
private:
    std::vector<char> lettersBag;
    Pool *m_pool;
};

#endif //LETTERSBAG_H
