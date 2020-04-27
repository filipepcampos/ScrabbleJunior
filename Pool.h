#include <fstream>
#include <string>
#include <vector>

#ifndef POOL_H
#define POOL_H

class Pool
{
public:
	Pool(const std::vector <char>& allLetters);
	~Pool();
	std::vector<char> getLettersBag()const;
	std::vector<char> getAllLetters()const;
	void showBag();
	void removeLetter(char letter);
	void addLetter();
private:
	std::vector <char> lettersBag;
	std::vector <char> allLetters;
};

#endif