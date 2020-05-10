#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iostream>

#ifndef POOL_H
#define POOL_H

class Pool
{
private:
    std::vector<char> allLetters;
public:
    explicit Pool(const std::vector<char>& letters);
    char draw();
};
#endif
