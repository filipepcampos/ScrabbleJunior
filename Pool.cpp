#include <iostream>
#include "Pool.h"
Pool::Pool(const std::vector <char>& letters)
{
	int number;
	for (int i = 0; i < 7; i++)
	{
		number = rand() % (letters.size());
		lettersBag.push_back(letters.at(number));
		letters.erase(letters.begin() + number);
	}
	allLetters = letters;
}
Pool::~Pool()
{
	lettersBag.clear();
}
std::vector<char> Pool::getLettersBag()const
{
	return lettersBag;
}
std::vector<char> Pool::getAllLetters()const
{
	return allLetters;
}
void Pool::removeLetter(char letter)
{
	lettersBag.erase(std::find(lettersBag.begin(), lettersBag.end(), toupper(letter)));
}
void Pool::addLetter()
{
	int number = rand() % (allLetters.size());
	lettersBag.push_back(allLetters.at(number));
	letters.erase(allLetters.begin() + number);

}
void Pool::showBag()
{
	for (int i = 0; i < 7; i++)
	{
		std::cout << lettersBag[i] << ',';
	}
	std::cout << std::endl;
}