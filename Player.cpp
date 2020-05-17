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
    int played_turns = 0;
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
            played_turns++;
            Utility::pressToContinue();
        }
        else if(played_turns == 1 && !m_board->gameOver()){
            skipTurn();
        }
    }
    letterBag.addLetter(played_turns);
    if(!played_turns){
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
    } while(points_won == -1); // m_board->play returns -1 when move was invalid
    letterBag.remove(play.letter);
    return points_won;
}

void Player::readPlay(detail::Play &play) const
{
    std::cout << std::endl << PLAYER_COLORS[id] << "Player " << id << "'s turn: " << RESET << std::endl;
    letterBag.showBag();
    std::vector<char> playable_letters = m_board->getPlayableLetters();

    std::cout << "Choose one letter(X) from your bag and the position(Aa) you want to place them (\"X-Aa\")";

    while( !(Utility::read(play, convertToPlay) && testPlay(play, playable_letters)) ){
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
    int tiles_to_exchange = std::max(letterBag.getSize(), m_pool->getSize());
    tiles_to_exchange = tiles_to_exchange > 2 ? 2 : tiles_to_exchange;

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
    do{
        std::cout << "\n\nPlease choose an letter to exchange: ";
        letterBag.showBag();
    } while(! (Utility::read(c) && testLetterToExchange(c)));
    return ::toupper(c);
}
bool Player::testLetterToExchange(const char &c) const{
    return isalpha(c) && letterBag.find(::toupper(c));
}

void Player::outputScore() const {
    std::cout << PLAYER_COLORS[id] << "\nScore: " << RESET << score << std::endl;
}
void Player::outputInvalid(){
    std::cout << "Invalid input, please re-enter" << std::endl;
    Utility::pressToContinue();
}

void Player::skipTurn() const{
    std::cout << PLAYER_COLORS[id] << "Turn skipped " << RESET << std::endl;
    Utility::pressToContinue();
}