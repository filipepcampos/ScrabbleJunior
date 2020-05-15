#include <algorithm>
#include <sstream>
#include "Player.h"
#include "Colors.h"
#include "Utility.h"

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
                return; // EOF Signal
            }
            score += points_won;
            outputScore();
            played++;
            Utility::pressToContinue();
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

int Player::playTurn() {
    detail::Play play{};
    int points_won = 0;
    do{
        m_board->print();
        if(!readPlay(play)){
            return -1; // EOF Signal
        };
        points_won = m_board->play(play.letter, play.vertical_char, play.horizontal_char);

        if(points_won == -1) {
            outputInvalid();
        }
    } while(points_won == -1);
    lettersBag.remove(play.letter);
    return points_won;
}

bool Player::canPlay() const{
    std::vector<char> playable_letters = m_board->getPlayableLetters();
    return lettersBag.intersects(playable_letters);
}

bool Player::readPlay(detail::Play &play) const
{
    std::cout << std::endl << PLAYER_COLORS[id] << "Player " << id << "'s turn: " << RESET << std::endl;
    lettersBag.showBag();
    std::vector<char> playable_letters = m_board->getPlayableLetters();

    std::cout << "Choose one letter(X) from your bag and the position(Aa) you want to place them (\"X-Aa\")";
    while(true){
        if(Utility::read(play, convertToPlay)){
            if(!play.letter){
                return false; // EOF has occurred if letter is empty
            }
            if(testPlay(play, playable_letters)){
                break;
            }
        }
        std::cout << "Invalid input, please re-enter (\"X-Aa\")";
    }
    return true;
}
bool Player::convertToPlay(detail::Play &play, const std::string &str) {
    std::stringstream ss{str};
    char delimiter;
    if(ss >> play.letter >> delimiter >> play.vertical_char >> play.horizontal_char){
        if(!ss.fail() && !ss.rdbuf()->in_avail() && delimiter == '-'){
            play.letter = ::toupper(play.letter);
            return true;
        }
    }
    return false;
}
bool Player::testPlay(const detail::Play &play, const std::vector<char> &playable_letters) const{
    return isupper(play.vertical_char) && islower(play.horizontal_char)
            && isalpha(play.letter)    && lettersBag.find(play.letter)
            && std::find(playable_letters.begin(), playable_letters.end(), play.letter) != playable_letters.end();
}

void Player::exchangeTiles() {
    int letters_bag_size = lettersBag.getSize();
    int tiles_to_exchange = letters_bag_size > 2 ? 2 : letters_bag_size;
    int can_draw = m_pool->numCanDraw();
    tiles_to_exchange = std::max(tiles_to_exchange, can_draw);
    for(int i = 0; i < tiles_to_exchange; i++){
        char c = readLetterToExchange();
        if(c){
            lettersBag.replaceTile(c);
        }
        else{
            break; // EOF has occurred
        }
    }
    lettersBag.addRandomLetter(tiles_to_exchange);
}


char Player::readLetterToExchange() const{
    char c;
    while(true){
        lettersBag.showBag();
        std::cout << "Please choose an letter to exchange: ";
        if(Utility::read(c) && testLetterToExchange(c)){
            if(!c || testLetterToExchange(c)){
                break;
            }
        }
    }
    return ::toupper(c);
}
bool Player::testLetterToExchange(const char &c) const{
    return !c || (isalpha(c) && lettersBag.find(::toupper(c)));
}

void Player::outputScore() const {
    std::cout << PLAYER_COLORS[id] << "Score: " << RESET << score << std::endl;
}
void Player::outputInvalid(){
    std::cout << "Invalid input, please re-enter" << std::endl;
    Utility::pressToContinue();
}

void Player::skipTurn() const{
    std::cout << PLAYER_COLORS[id] << "Turn skipped " << RESET << std::endl;
    Utility::pressToContinue();
}