//card.cpp

#include "card.h"

// Card
// Input: price, prestige, gemColor 
// Description: Constructs cards based on input
// Output: None
Card::Card(int price[5], int prestige, string gemColor) {
    for (int i = 0; i < 5; i++) {
        this->price[i] = price[i];
    }
    this->prestige = prestige;
    this->gemColor = gemColor;
}

// getGemColor
// Input: none 
// Description: Returns the gemColor of a card
// Output: None
string Card::getGemColor() {
    return gemColor;
}

// getPrestige
// Input: none 
// Description: Returns the prestige number of a card
// Output: None
int Card::getPrestige() {
    return prestige;
}

// getPrice
// Input: none 
// Description: Returns the price of the card
// Output: None
int Card::getPrice(int i) {
    return price[i];
}