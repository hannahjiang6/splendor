/* 
 * card.h 
 * CS11 Splendor
 */
#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <string>
#include "termfuncs.h"
using namespace std;

class Card {
public:
    //Constructor
    Card(int price[5], int prestige, string gemColor);

    //Getters
    string getGemColor();
    int getPrestige();
    int getPrice(int price);

private:
    int price[5];
    int prestige;
    string gemColor;
};

#endif
