#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <sstream>
#include <iostream>

namespace Utility{
    /**
     * Wait for user input to continue
     * @param msg
     */
    void pressToContinue(const std::string &msg = "Press any key to continue...");

    /**
     * Thrown when read() detects EOF
     */
    class CinEofError : public std::exception{};

    /**
     * Converter used by read() by default
     * @tparam T
     * @param var
     * @param str
     * @return true if successful
     */
    template <typename T>
    bool defaultConverter(T &var, const std::string &str){
        std::stringstream ss{str};
        return ss >> var && !ss.rdbuf()->in_avail();
    }

    /**
     * Read a string from cin and convert it to type T using converter function
     * @tparam T
     * @param var
     * @param convert - conversion function that converts std::string to type T, returning true if successful
     * @return true if read was successful
     */
    template <typename T>
    bool read(T &var, bool (*convert)(T &var, const std::string &str) = defaultConverter){
        std::cout << "\n> ";
        std::string buffer;
        std::getline(std::cin, buffer);
        if (std::cin.eof()) {
            throw CinEofError();
        }
        return (*convert)(var, buffer); // True if successful
    }
}

#endif
