#ifndef LETTERBAG_H
#define LETTERBAG_H
#include <vector>
#include "Pool.h"

class LetterBag{
public:
    explicit LetterBag(Pool &m_pool);

    /**
     * Add n letters taken randomly from m_pool if available
     * @param n - letters to attempt to draw
     * @return (int) number of letters actually drawn
     */
    int addLetter(int n = 1);

    /**
     * Show all letters in letterBag
     */
    void showBag() const;

    /**
     * Remove a letter from letterBag
     * @param c
     */
    void remove(char c);

    /**
     * Remove a letter from letterBag and replace it in the pool
     * @param c
     */
    void replaceTile(char c);

    /**
     * Get number of letters available
     * @return
     */
    int getSize() const;

    /**
     * Find char c in letterBag
     * @param c
     * @return true if found
     */
    bool find(char c) const;

    /**
     * Check if any letter from letterBag is present in vec
     * @param vec
     * @return true if present
     */
    bool intersects(const std::vector<char> &vec) const;
private:
    std::vector<char> letterBag;
    Pool *m_pool;

    /**
     * Generic overload of find, try to find char c in vec
     * @param c
     * @param vec
     * @return true if found
     */
    bool find(char c, const std::vector<char> &vec) const;
};

#endif
