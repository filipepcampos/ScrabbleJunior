#include <algorithm>
#include <sstream>
#include "Player.h"
#include "Colors.h"

int Player::id_counter = 0;

Player::Player(Board& board, Pool& pool) : m_board{&board}, m_pool{&pool}, score{0}, id{++id_counter}, lettersBag{pool}{}
Player::~Player(){
    id_counter = 0;
}
int Player::getScore() const{
    return score;
}
int Player::getId() const{
    return id;
}

void Player::play()
{
    int played = 0;
    for(int i = 0; i < 2; i++){
        if(canPlay()){
            int points_won = playTurn();
            if(points_won == -1){
                return;
            }
            score += points_won;
            outputScore();
            played++;
            pressToContinue();
        }
        else{
            skipTurn();
        }
    }
    lettersBag.addRandomLetter(played);
    if(!played){
       exchangeTiles();
    }
}

void Player::exchangeTiles() {
    int letters_bag_size = lettersBag.getSize();
    int tiles_to_exchange = letters_bag_size > 2 ? 2 : letters_bag_size;
    for(int i = 0; i < tiles_to_exchange; i++){
        if(m_pool->canDraw()){
            char c = readLetterToExchange();
            if(c){
                lettersBag.exchangeTile(c);
            }
            else{
                // EOF has occurred
                break;
            }
        }
    }
}

int Player::playTurn() {
    char letter, vertical_char, horizontal_char;
    int points_won = 0;
    do{
        m_board->print();
        if(!readInput(letter, vertical_char, horizontal_char)){
            return -1;
        };
        points_won = m_board->play(letter, vertical_char, horizontal_char);

        if(points_won == -1) {
            outputInvalid();
        }
    } while(points_won == -1);
    lettersBag.remove(letter);
    return points_won;
}

bool Player::canPlay() const{
    std::vector<char> playable_letters = m_board->getPlayableLetters();
    return lettersBag.intersects(playable_letters);
}

void Player::pressToContinue(){
    std::cout << "Press any key to continue..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void Player::outputScore() const {
    std::cout << PLAYER_COLORS[id] << "Score: " << RESET << score << std::endl;
}
void Player::outputInvalid(){
    std::cout << "Invalid input, please re-enter" << std::endl;
}

void Player::skipTurn() const{
    std::cout << PLAYER_COLORS[id] << "Turn skipped " << RESET << std::endl;
    pressToContinue();
}

bool Player::readInput(char &letter, char &vertical_char, char &horizontal_char) const
{
    std::cout << std::endl << PLAYER_COLORS[id] << "Player " << id << "'s turn: " << RESET << std::endl;
    lettersBag.showBag();
    std::vector<char> playable_letters = m_board->getPlayableLetters();

    char delimiter;
    std::cout << "Choose one letter(X) from your bag and the position(Aa) you want to place them\n(\"X-Aa\") :  ";
    while (!(std::cin >> letter >> delimiter >> vertical_char >> horizontal_char)
        || !isupper(vertical_char)|| !islower(horizontal_char)|| !isalpha(letter) || delimiter != '-'
        || !lettersBag.find(::toupper(letter))
        || std::find(playable_letters.begin(), playable_letters.end(), ::toupper(letter)) == playable_letters.end())
    {
        if(std::cin.eof()){
            return false;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input, please re-enter." << std::endl << "(\"X-Aa\") :  ";
    }
    letter = ::toupper(letter);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return true;
}

char Player::readLetterToExchange(){
    std::string buffer;
    do{
        lettersBag.showBag();
        std::cout << "Please choose an letter to exchange: ";
        std::getline(std::cin, buffer);
        if(std::cin.eof()){
            return 0;
        }
    } while (buffer.size() != 1 || !isalpha(buffer[0]) || !lettersBag.find(::toupper(buffer[0])));
    return ::toupper(buffer[0]);
}
