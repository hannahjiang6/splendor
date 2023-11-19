/*
 * game.h
 * CS11 Splendor
 */

#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <string>
#include "termfuncs.h"
#include "card.h"
#include "player.h"
#include "vector.h"
using namespace std;

class Game {
  public:
    //constructor
    Game(string filename, string play_nobles);

    // Destructor
    ~Game();

    //main gameplay
    void playGame();

    //utility functions
    string indexToColor(int index);

  private:
    //drawing functions
    void drawBoard();
    void drawNoble(int vpos, int hpos, Card *c);
    void drawPlayer(int vpos, int hpos, Player *p);
    void drawDeck(int vpos, int hpos, string type);
    void drawCard(int vpos, int hpos, Card *c);
    void drawGem(int vpos, int hpos, string color);
    void clearBoard();

    static const int BOARD_HEIGHT = 41;
    static const int BOARD_WIDTH = 120;
    static const int CARD_WIDTH = 13;
    static const int CARD_HEIGHT = 8;
    static const int CARD_ROWS = 3;
    static const int CARD_COLS = 4;
    static const int NUM_NOBLES = 3;
    static const int GEMS = 6;

    //nobles activated true/false
    bool play_nobles;

    //cards in play
    Card *grid[CARD_ROWS][CARD_COLS];

    //display
    char board[BOARD_HEIGHT][BOARD_WIDTH];
    string board_color[BOARD_HEIGHT][BOARD_WIDTH];

    //Gem colors in the game
    string colors[GEMS];

    Vector mines;
    Vector transport;
    Vector vendors;
    Vector nobles;
    int player_num;

    int gem_bank[GEMS];

    Player player_1;
    Player player_2;

    // Functions to initialize the cards
    void init_mines(string filename);
    void init_transport(string filename);
    void init_vendors(string filename);
    void init_deck();

    // Functional functions
    int gem_index(string color);

    // Get whose turn it is
    Player *get_player(int num);

    // Checkers
    bool player_win();
    void gem_count();
    bool validCard(char type, int index);
    bool buyCard(Card *card);

    
    // Over 10 gems specific functions
    void return_gems();

    // Specific Game Moves
    bool query();
    bool reserve();
    bool pick3();
    bool pick2();
    bool buy();
    bool buy_reserve();
};

#endif
