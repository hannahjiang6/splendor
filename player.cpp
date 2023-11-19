// player.cpp

#include "player.h"

// Player
// Input: none 
// Description: Initializes and sets everything to 0/null
// Output: None
Player::Player() {
    prestige = 0;
    for (int i = 0; i < 5 ; ++i) {
        discounts[i]= 0;
    }
    for (int i = 0; i < 6 ; ++i) {
        gems[i]= 0;
    }
    for (int i = 0; i < 3 ; ++i) {
        reserveCards[i] = nullptr;
    }
}

// ~Player
// Input: none 
// Description: Destructs the reserveCards
// Output: None
Player::~Player() {
    for (int i = 0; i < 3; i++) {
        delete reserveCards[i];
    }
}

void Player::setPrestige(int i) {
    prestige = prestige + i;
}

void Player::setDiscount(int color) {
    discounts[color]++;
}


void Player::setGems(int i) {
    gems[i]++;
}

void Player::setReserve(Card *reserve) {
    for (int i = 0; i < 3; i++) {
        if (reserveCards[i] == nullptr) {
            reserveCards[i] = reserve;
            return;
        }
    }
}

void Player::removeGems(int i) {
    gems[i]--;
}

void Player::removeReserve(int i) {
    if (reserveCards[i] != nullptr) {
        reserveCards[i] = nullptr;
        return;
    }
    
}

int Player::totalPrestige() {
    return prestige;
}   

int Player::getDiscount(int i) {
    return discounts[i];
}

int Player::getGemCount(int i) {
    return gems[i];
}

Card *Player::getReserve(int i) {
    return reserveCards[i];
}



