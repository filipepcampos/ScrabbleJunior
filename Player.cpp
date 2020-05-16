#include <algorithm>
#include <sstream>
#include "Player.h"
#include "Colors.h"
#include "Utility.h"

int Player::id_counter = 0;

Player::Player(Board& board, Pool& pool) : m_board{&board}, m_pool{&pool}, score{0}, id{++id_counter}, letterBag{pool}{}
Player::~Player(){
    id_counter = 0;
}
int Player::getScore() const{
    return score;
}
int Player::getId() const{
    return id;
}

bool Player::canPlay() const{
    std::vector<char> playable_letters = m_board->getPlayableLetters();
    return letterBag.intersects(playable_letters);
}

void Player::play()
{
    int played = 0;
    for(int i = 0; i < 2; i++){
        if(canPlay()){
            int points_won = 0;
            try{
                points_won = playTurn();
            } catch(const Utility::CinEofError &e){
                return;
            }
            score += points_won;
            outputScore();
            played++;
            Utility::pressToContinue();
        }
        else if(played == 1){
            skipTurn();
        }
    }
    letterBag.addLetter(played);
    if(!played){
       exchangeTiles();
    }
}

int Player::playTurn() {
    detail::Play play{};
    int points_won = 0;
    do{
        m_board->print();
        readPlay(play);
        points_won = m_board->play(play.letter, play.vertical_char, play.horizontal_char);
        if(points_won == -1) {
            outputInvalid();
        }
    } while(points_won == -1);
    letterBag.remove(play.letter);
    return points_won;
}

void Player::readPlay(detail::Play &play) const
{
    std::cout << std::endl << PLAYER_COLORS[id] << "Player " << id << "'s turn: " << RESET << std::endl;
    letterBag.showBag();
    std::vector<char> playable_letters = m_board->getPlayableLetters();

    std::cout << "Choose one letter(X) from your bag and the position(Aa) you want to place them (\"X-Aa\")";
    while(true){
        if(Utility::read(play, convertToPlay) && testPlay(play, playable_letters)){
            break;
        }
        std::cout << "Invalid input, please re-enter (\"X-Aa\")";
    }
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
           && isalpha(play.letter) && letterBag.find(play.letter)
           && std::find(playable_letters.begin(), playable_letters.end(), play.letter) != playable_letters.end();
}

void Player::exchangeTiles() {
    int letters_bag_size = letterBag.getSize();
    int tiles_to_exchange = letters_bag_size > 2 ? 2 : letters_bag_size;
    tiles_to_exchange = std::max(tiles_to_exchange, m_pool->numCanDraw());

    std::cout << CLEAR;
    m_board->print();
    std::cout << std::endl << PLAYER_COLORS[id] << "Player " << id << RESET
              <<", exchange " << tiles_to_exchange << " letters:\n";
    for(int i = 0; i < tiles_to_exchange; i++){
        char c;
        try{
            c = readLetterToExchange();
        } catch(const Utility::CinEofError &e){
            return;
        }
        letterBag.replaceTile(c);
    }
    letterBag.addLetter(tiles_to_exchange);
}

char Player::readLetterToExchange() const{
    char c;
    while(true){
        std::cout << "\nPlease choose an letter to exchange: ";
        letterBag.showBag();
        if(Utility::read(c) && testLetterToExchange(c)){
            break;
        }
        std::cout << std::endl;
    }
    return ::toupper(c);
}
bool Player::testLetterToExchange(const char &c) const{
    return isalpha(c) && letterBag.find(::toupper(c));
}

void Player::outputScore() const {
    std::cout << PLAYER_COLORS[id] << "Score: " << RESET << score << std::endl;
}
void Player::outputInvalid(){
    std::cout << "Invalid input, please re-enter" << std::endl;
    Utility::pressToContinue();
}

void Player::skipTurn() const{
    std::cout << PLAYER_COLORS[id] << "Turn skipped " << RESET << std::endl << std::endl;
    Utility::pressToContinue();
}