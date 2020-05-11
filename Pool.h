#ifndef POOL_H
#define POOL_H
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <iostream>

class Pool
{
private:
    std::vector<char> allLetters;
public:
    explicit Pool(const std::vector<char>& letters);
    char draw();
};
#endif
