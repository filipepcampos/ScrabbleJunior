#ifndef POOL_H
#define POOL_H
#include <vector>

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
     * How many tiles are available
     */
    int getSize() const;

    /**
     * Add a new letter to the pool, reshuffling it
     * @param c
     */
    void add(char c);
};
#endif
