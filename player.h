/*
 * player.h
 * CS11 Splendor
 */

#ifndef PLAYER_H
#define PLAYER_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "card.h"
using namespace std;

class Player {
public:
    Player(); // Constructor
    ~Player(); // Deconstructor

    // Getters
    int totalPrestige();
    int getDiscount(int color);
    int getGemCount(int i);
    Card *getReserve(int i);

    // Setters
    void setReserve(Card *reserve);
    void setGems(int i);
    void setDiscount(int color);
    void setPrestige(int points);

    // Remove gems from the player
    void removeGems(int i);
    void removeReserve(int i);

private:
    int gems[6];
    int prestige;
    int discounts[5];
    Card *reserveCards[3];

};

#endif
