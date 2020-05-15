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

    /**
     * Shuffle all chars in vector
     * @param vec
     */
    static void shuffle(std::vector<char>& vec);
public:
    explicit Pool(const std::vector<char>& letters);

    /**
     * Remove a letter from pool
     * @return letter
     */
    char draw();

    /**
     * How many tiles can a player draw
     */
    int numCanDraw();

    /**
     * Add a new letter to the pool, reshuffling it
     * @param c
     */
    void add(char c);
};
#endif
