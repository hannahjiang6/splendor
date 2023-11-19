/*
 * game.cpp
 * CS11 Splendor
 */

#include "game.h"
#include <fstream>
#include <iostream>

// Game
// Input: file and input 
// Description: Creates the cards and sets up the game
// Output: None
Game::Game(string filename, string play_nobles){
    ifstream infile(filename);
    if (infile.fail()) {
        cerr << "Failed to open deck file: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    //store gem colors for easy reference in drawing functions 
    colors[0] = "white";
    colors[1] = "red";
    colors[2] = "pink";
    colors[3] = "blue";
    colors[4] = "green";
    colors[5] = "gold";
    //include nobles or not
    if (play_nobles == "true") {
            this->play_nobles = true;
    } else {
            this->play_nobles = false;
    }
    //initialize the board
    for(int i = 0; i < BOARD_HEIGHT; i++){
            for(int j = 0; j < BOARD_WIDTH; j++){
                board[i][j] = ' ';
                board_color[i][j] = "white";
            }
    }     
    init_mines(filename);
    init_transport(filename);
    init_vendors(filename);
    init_deck();
}

// ~Game
// Input: none 
// Description: Clears up valgrind and makes sure there are no leaks
// Output: None
Game::~Game() {
    int v_size = vendors.size();
    int m_size = mines.size();
    int t_size = transport.size();

    for (int i = 0; i < m_size; i++) {
        delete mines.at(i);
    }
    for (int i = 0; i < t_size; i++) {
        delete transport.at(i);
    }
    for (int i = 0; i < v_size; i++) {
        delete vendors.at(i);
    }
    for (int i = 0; i < 3; i++) { // delete the grid
        for (int j = 0; j < 4; j++) {
            delete grid[i][j];
        }
    }
}

//main game loop
void Game::playGame(){
    bool game_over = false;
    bool player1_turn = true;
    while(!game_over){
        clearBoard(); //reset the screen
        screen_clear();
        screen_home();
        drawBoard(); //draw the board
        if (player1_turn) { // Print whose turn it is
            cout << "PLAYER 1, enter your next move: ";
            player_num = 1;
            if (query() == true) {
                game_over = true;
            }
        } else {
            cout << "PLAYER 2, enter your next move: ";
            player_num = 2;
            if (query() == true) {
                game_over = true;
            }
        }
        if (player_win() == true) { // player win function
            game_over = true;
        }
        gem_count(); // check if a player has over 10 gems
        player1_turn = !player1_turn; //change turns
    } 
}

// player_win
// Input: none
// Description: Check to see if a person has won or not
// Output: The winner!
bool Game::player_win() {
    if (get_player(player_num)->totalPrestige() == 15) {
        if (player_num == 1) {
            player_num = 2;
            clearBoard();
            screen_clear();
            screen_home();
            drawBoard();
            cout << "PLAYER 2, enter your next move: "; 
            query();
            clearBoard();
            screen_clear();
            screen_home();
            drawBoard();
            if (player_2.totalPrestige() != 15) {
                cout << "Congratulations Player 1: YOU WIN!!!" << endl;
                return true;
            } else {
                cout << "You losers tied - BORING!!!!" << endl;
                return true;
            }
        } else {
            clearBoard();
            screen_clear();
            screen_home();
            drawBoard();
            cout << "Congratulations Player 2: YOU WIN!!!!" << endl;
            return true;
        }
        return false;
    }
    return false;
}

// get_player
// Input: num
// Description: gets the whichever player's turn
// Output: returns the address of whichever player's turn it is
Player *Game::get_player(int num) {
    if (num == 1) {
        return &player_1;
    } else if (num == 2) {
        return &player_2;
    } else {
        return nullptr;
    }
}

// gem_count
// Input: none
// Description: check if a player has over 10 gems
// Output: none
void Game::gem_count() {
    int total_gems = 0;
    for (int i = 0; i < 6; i++) {
        total_gems = total_gems + get_player(player_num)->getGemCount(i); 
    }

    while (total_gems > 10) {
        clearBoard();
        screen_clear();
        screen_home();
        drawBoard();
        return_gems();
        
        total_gems = 0;
        for (int i = 0; i < 6; i++) {
            total_gems = total_gems + get_player(player_num)->getGemCount(i); 
        }
    }
}

// return_gems
// Input: none
// Description: if a player has over 10 gems 
// return a gem back to the gem_bank
// Output: none
void Game::return_gems() {
    string color;
    int type;

    cout << "PLAYER " << player_num << ", you have more than 10 gems! ";
    cout << "Name one gem you would like to return: ";
    cin >> color;

    type = gem_index(color);

    while (type == -1) {
        cout << "Invalid color. Name one gem you would like to return: ";
        cin >> color;
        type = gem_index(color);
    }

    if (type != -1) {
        if (get_player(player_num)->getGemCount(type) > 0) {
            gem_bank[type]++;
            get_player(player_num)->removeGems(type);
            return; 
        } else {
            cout << "Invalid color. Name one gem you would like to return: ";
            cin >> color;
            type = gem_index(color);
        }
    } 
}

// gem_index
// Input: string of the input
// Description: match the gem input into it's matching respective int
// Output: returns an int
int Game::gem_index(string color) {
    if (color == "white") {
        return 0;
    } else if (color == "red") {
        return 1;
    } else if (color == "pink") {
        return 2;
    } else if (color == "blue") {
        return 3;
    } else if (color == "green") {
        return 4;
    } else if (color == "gold") {
        return 5;
    }
    return -1;
}

// query
// Input: none 
// Description: Handles whatever the player inputs for their game choice
// Output: None
bool Game::query() {
    string query;
    bool valid = false;
    cin >> query;
    if (query == "q" || query == "p3" || query == "p2" || 
        query == "b" || query == "br" || query == "r") {
        valid = true; // Making sure the query is valid
    } else {
        valid = false;
    }
    while (!valid) { // If not valid, keep prompting
        cout << "Invalid move - enter a valid move: ";
        cin >> query;
        if (query == "q" || query == "p3" || query == "p2" || 
            query == "b" || query == "br" || query == "r") {
            valid = true;
        }
    }
    if (query == "q") {
        return true;
    } else if (query == "p3") { 
        return pick3();
    } else if (query == "p2") {
        return pick2();
    } else if (query == "b") {
        return buy();
    } else if (query == "br") {
       return buy_reserve();
    } else if (query == "r") {
        return reserve();
    }
    return false;
}

// buy
// Input: none
// Description: sends in different cards to the buyCard function
// Output: bool if command was succesful
bool Game::buy() {
    char type;
    int index;
    cin >> type >> index;
    if (validCard(type, index) == true) {
        if (type == 'm') {
            Card *buy_mcard = grid[2][index-1];
            if (buyCard(buy_mcard) == true) {
                delete buy_mcard;
                grid[2][index-1] = mines.back();    
                mines.pop_back();
            } else {
                return query();
            }
        } else if (type == 't') {
            Card *buy_tcard = grid[1][index-1];
            if (buyCard(buy_tcard) == true) {
                delete buy_tcard;
                grid[1][index-1] = transport.back();
                transport.pop_back();
            } else {
                return query();
            }
        } else if (type == 'v') {
            Card *buy_vcard = grid[0][index-1];
            if (buyCard(buy_vcard) == true) {
                delete buy_vcard;
                grid[0][index-1] = vendors.back();
                vendors.pop_back();
            } else {
                return query();
            }
        }
        return false;
    } else {
        return query();
    }
}

// buyCard
// Input: a pointer to the card on the grid
// Description: Add the information from the bought card to the player's
// information
// Output: bool if the card can be bought
bool Game::buyCard(Card *card) {
    int card_price[5] = {0, 0, 0, 0, 0};
    int player_gems[5] = {0, 0, 0, 0, 0};
    int discount[5] = {0, 0, 0, 0, 0};
    int pd_gems[5] = {0, 0, 0, 0, 0};
    int left[5] = {0, 0, 0, 0, 0};
    int discounted_gem = -1;
    int ptotal = 0;
    int ctotal = 0;
    int gem_amount = 0;
    int counter = 0;
    int leftover = 0;
    int discount_temp = 0;
    int gold_gems = get_player(player_num)->getGemCount(5);

    for (int i = 0; i < 5; i++) {
        card_price[i] = card->getPrice(i);
        player_gems[i] = get_player(player_num)->getGemCount(i);
        discount[i] = get_player(player_num)->getDiscount(i);
        pd_gems[i] = player_gems[i] + discount[i];
    }
    for (int j = 0; j < 5; j++) { 
        if (card_price[j] <= pd_gems[j]) {
            counter++;
        }
    }
    if (counter == 5) {
        // add prestige
        get_player(player_num)->setPrestige(card->getPrestige()); 
        // add the discount
        discounted_gem = gem_index(card->getGemColor()); 
        get_player(player_num)->setDiscount(discounted_gem);
        for (int i = 0; i < 5; i++) {
            if (card_price[i] != 0) {
                gem_amount = card_price[i];
                discount_temp = discount[i];
            }
            for (int k = 0; k < gem_amount; k++) {
                get_player(player_num)->removeGems(i);
                gem_bank[i]++;
            }
            for (int j = 0; j < discount_temp; j++) {
                get_player(player_num)->setGems(i);
                gem_bank[i]--;
            }
            gem_amount = 0;
            discount_temp = 0;
        }
        return true;
    } else {
        for (int i = 0; i < 5; i++) {
            if (card_price[i] <= pd_gems[i]) {
                left[i] = -1;
            } else {
                left[i] = 1;
            }
            if (left[i] == 1) {
                ptotal = ptotal + pd_gems[i];
                ctotal = card_price[i];
            }
        }
        leftover = ctotal - ptotal;
        if (leftover <= gold_gems) {
            get_player(player_num)->setPrestige(card->getPrestige()); 
            discounted_gem = gem_index(card->getGemColor()); 
            get_player(player_num)->setDiscount(discounted_gem);

            //removes the gold gems
            for (int h = 0; h < leftover; h++) {
                get_player(player_num)->removeGems(5);
                gem_bank[5]++;
            }
            for (int i = 0; i < 5; i++) {
                if (card_price[i] != 0) {
                    gem_amount = card_price[i];
                    discount_temp = discount[i];
                }
                for (int k = 0; k < gem_amount; k++) {
                    while(get_player(player_num)->getGemCount(i) != 0) {
                        get_player(player_num)->removeGems(i);
                        gem_bank[i]++;
                    }
                    gem_amount = 0;
                    discount_temp = 0;
                    return true;
                }
            }
        } else {
            cout << "Invalid move (cannot afford card) ";
            cout << "- enter a valid move: ";
            return false;
        }
        return false;
    } 
    return false;
}

// buy_reserve
// Input: none
// Description: buy a card from the a player's reserve index
// Output: bool if the card is valid
bool Game::buy_reserve() {
    int index;
    cin >> index;

    if (get_player(player_num)->getReserve(index-1) != nullptr) {
        Card *br = get_player(player_num)->getReserve(index-1);
        if (buyCard(br) == true) {
            delete br;
            get_player(player_num)->removeReserve(index-1);
            return false;
        } else {
            return query();
        }
    } else {
        cout << "Invalid move (no card at that index) ";
        cout << "- enter a valid move: ";
        return query();
    }
    return false;
}

// reserve
// Input: none
// Description: Reserve a card from the grid into the players
// reserve pile
// Output: bool if the card is valid
bool Game::reserve() {
    char type;
    int index;
    cin >> type >> index;

    if (get_player(player_num)->getReserve(2) == nullptr) {
        if (validCard(type, index) == true) {
            gem_bank[5]--;
            get_player(player_num)->setGems(5); 
            if (type == 'm') {
                get_player(player_num)->setReserve(grid[2][index-1]);
                grid[2][index-1] = mines.back();
                mines.pop_back();
            } else if (type == 't') {
                get_player(player_num)->setReserve(grid[1][index-1]);
                grid[1][index-1] = transport.back();
                transport.pop_back();
            } else if (type == 'v') {
                get_player(player_num)->setReserve(grid[0][index-1]);
                grid[0][index-1] = vendors.back();
                vendors.pop_back();
            }
            return false;
        } else {
            return query();
        }
    } else {
        cout << "Invalid move (reserve full) - enter a valid move: ";
        return false;
    }
}

// validCard
// Input: type and index of the card 
// Description: Check if the card a player picked from the board is valid
// Output: bool if the card is valid
bool Game::validCard(char type, int index) { 
    if (type == 'm' || type == 'v' || type == 't') {
        if (index < 5 && index > 0) {
            return true;
        } else {
            cout << "Invalid move (invalid card column) - ";
            cout << "enter a valid move: ";
            return false;
        }
    } else {
        cout << "Invalid move (invalid card type) - enter a valid move: ";
        return false;
    }
}

// pick3
// Input: none 
// Description: Adds 3 different gems to the player's gem array
// Output: bool if the gems picked are valid
bool Game::pick3() {
    string c1, c2, c3 = "";
    int g1, g2, g3;

    cin >> c1 >> c2 >> c3;
    g1 = gem_index(c1);
    g2 = gem_index(c2);
    g3 = gem_index(c3);

    if (g1 != -1 && g1 != 5 && g2 != -1 && g2 != 5 && g3 != -1 && g3 != 5) {
        if (g1 != g2 && g2 != g3 && g1 != g3) {
            if (gem_bank[g1] > 0 && gem_bank[g2] > 0 && gem_bank[g3] > 0) {
                gem_bank[g1]--;
                gem_bank[g2]--;
                gem_bank[g3]--;
                get_player(player_num)->setGems(g1);
                get_player(player_num)->setGems(g2);
                get_player(player_num)->setGems(g3);
                return false;
            } else {
                cout << "Invalid move (unavailable gems) - ";
                cout << "enter a valid move: ";
                return query();
            }
        } else {
            cout << "Invalid move (duplicate color) - enter a valid move: ";
            return query();
        }
    } else {
        cout << "Invalid move (invalid color) - enter a valid move: ";
        return query();
    }
}

// pick2
// Input: none 
// Description: Adds 2 of the same gem to the player's gem array
// Output: bool if the gems picked are valid
bool Game::pick2() {
    string c1 = "";
    int g1;
    
    cin >> c1;
    g1 = gem_index(c1);

    if (g1 != -1 && g1 != 5) {
        if (gem_bank[g1] == 4) {
            gem_bank[g1]--;
            gem_bank[g1]--;

            get_player(player_num)->setGems(g1);
            get_player(player_num)->setGems(g1);
            return false;
        } else {
            cout << "Invalid move (unavailable gems) - ";
            cout << "enter a valid move: ";
            return query();
        }
    } else {
        cout << "Invalid move (invalid color) - enter a valid move: ";
        return query();
    }
}

// init_mines
// Input: filename 
// Description: Reads in the data to create the mines deck
// Output: None
void Game::init_mines(string filename) {
    ifstream infile(filename);

    char card_type;
    int prestige, white, red, pink, blue, green;
    string discount;
    string skip;

    // Skip over reading the first line
    infile >> skip >> skip >> skip >> skip >> skip >> skip >> skip >> skip;

    // Read in the mine cards
    for (int i = 0; i < 40; i++) {
        infile >> card_type >> prestige >> discount;
        infile >> white >> red >> pink >> blue >> green;
        int price[5] = {white, red, pink, blue, green};
        Card *m_card = new Card(price, prestige, discount);
        mines.push_back(m_card);
    }

    infile.close();
}

// init_transport
// Input: filename 
// Description: Reads in the data to create the transport deck
// Output: None
void Game::init_transport(string filename) {
    ifstream infile(filename);

    char card_type;
    int prestige, white, red, pink, blue, green;
    string discount;
    string skip;

    infile >> skip >> skip >> skip >> skip >> skip >> skip >> skip >> skip;

    for (int i = 0; i < 40; i++) {
        infile >> skip >> skip >> skip >> skip >> skip >> skip >> skip >> skip;
    }

    // Read in the transportation cards
    for (int i = 40; i < 70; i++) {
        infile >> card_type >> prestige >> discount;
        infile >> white >> red >> pink >> blue >> green;
        int price[5] = {white, red, pink, blue, green};
        Card *t_card = new Card(price, prestige, discount);
        transport.push_back(t_card);
    }

    infile.close();
}

// init_vendors
// Input: filename 
// Description: Reads in the data to create the vendors deck
// Output: None
void Game::init_vendors(string filename) {
    ifstream infile(filename);

    char card_type;
    int prestige, white, red, pink, blue, green;
    string discount;
    string skip;

    infile >> skip >> skip >> skip >> skip >> skip >> skip >> skip >> skip;

    for (int i = 0; i < 70; i++) {
        // Skip over reading the first line
        infile >> skip >> skip >> skip >> skip >> skip >> skip >> skip >> skip;
    }

    // Read in the vendor cards
    for (int i = 70; i < 90; i++) {
        infile >> card_type >> prestige >> discount;
        infile >> white >> red >> pink >> blue >> green;
        int price[5] = {white, red, pink, blue, green};
        Card *v_card = new Card(price, prestige, discount);
        vendors.push_back(v_card);
    }

    infile.close();
}

// init_deck
// Input: None 
// Description: Creates the decks and initializes the gem_bank
// Output: None
void Game::init_deck() {
    // Drawing the cards into the grid
    for (int i = 0; i < 4; i++) {
        grid[2][i] = mines.back();
        mines.pop_back();
    }

    for (int i = 0; i < 4; i++) {
        grid[1][i] = transport.back();
        transport.pop_back();   
    }

    for (int i = 0; i < 4; i++) {
        grid[0][i] = vendors.back();
        vendors.pop_back();
    }

    // Initializing the gembank
    for (int i = 0; i < 5; i++) {
        gem_bank[i] = 4;
        for (int j = 5; j < 6; j++) {
            gem_bank[j] = 5;
        }
    }
}

//Get the gem color associated with the given index 
string Game::indexToColor(int index){
    return colors[index];
}

//draw all of the elements onto the game board
void Game::drawBoard(){
    int vpos = 0;
    int hpos = 1;

    //draw the card decks
    if(!vendors.empty()){
        drawDeck(vpos, hpos, "Vendors");
    }
    vpos += CARD_HEIGHT + 1;
    if(!transport.empty()){
        drawDeck(vpos, hpos, "Transport");
    }
    vpos += CARD_HEIGHT + 1;
    if(!mines.empty()){
        drawDeck(vpos, hpos, "Mines");
    }

    //draw the in play cards
    vpos = 0;
    hpos = CARD_WIDTH + 2;
    for(int i = 0; i < CARD_ROWS; i++){
        for(int j = 0; j < CARD_COLS; j++){
            if(grid[i][j] != nullptr){
                drawCard(vpos, hpos, grid[i][j]);
            }
            hpos += CARD_WIDTH + 1;
        }
        vpos += CARD_HEIGHT + 1;
        hpos = CARD_WIDTH + 2;
    }

    //draw the gems
    int gemOffset = ((CARD_WIDTH + 1) * 5) + 3;
    vpos = 1;
    hpos = gemOffset;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 2; j++){
            int index = (i * 2) + j;
            if(gem_bank[index] > 0){
                board[vpos][hpos + 5] = gem_bank[index] + 48;
                board_color[vpos][hpos + 5] = indexToColor(index);
                drawGem(vpos + 1, hpos, indexToColor(index));
            }
            hpos += CARD_WIDTH;
        }
        vpos += CARD_HEIGHT + 1;
        hpos = gemOffset;
    }
    
    //draw players
    drawPlayer(vpos + 1, 1, &player_1);
    drawPlayer(vpos + 1, ((CARD_WIDTH + 1) * 4), &player_2);

    //draw nobles
    if(play_nobles){
        hpos = ((CARD_WIDTH + 1) * 7) + 2;
        vpos = 2;
        for(int i = 0; i < NUM_NOBLES; i++){
            if(nobles.at(i) != nullptr){
                drawNoble(vpos, hpos, nobles.at(i));
            }
            vpos += CARD_HEIGHT + 1;
        }
    }

    //print out the finished board
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            if(board_color[i][j] != "white"){
                screen_fg(board_color[i][j]);
            }
            cout << board[i][j];
            screen_fg("white");
        }
        cout << endl;
   } 
   cout << endl; 
}

//draws a noble at the specified position of the board
void Game::drawNoble(int vpos, int hpos, Card *c){
    string top    = "|\\  /*\\  /|";
    string second = "| \\/   \\/ |";
    string blank  = "N         N";
    string bottom = "~~~~~~~~~~~";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];

        for(int j = 2; j < 5; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + 5][hpos + i] = bottom[i];
    }

    //draw the prices
    vpos += 2;
    for(int i = 0; i < GEMS-1; i++){
        int price = c->getPrice(i);
        if(price > 0){
            board_color[vpos][hpos + 4] = indexToColor(i);
            board_color[vpos][hpos + 5] = indexToColor(i);
            board_color[vpos][hpos + 6] = indexToColor(i);
            board[vpos][hpos + 4] = '|';
            board[vpos][hpos + 5] = price + 48;
            board[vpos][hpos + 6] = '|';
            vpos++;
        }
    }
}

//draws a player at the specified position of the board
void Game::drawPlayer(int vpos, int hpos, Player *p){
    //player string
    string prestige = to_string(p->totalPrestige());
    string player_s;
    if(hpos < 10){
        player_s = " PLAYER 1: " + prestige + " prestige point(s)";
    }else{
        player_s = " PLAYER 2: " + prestige + " prestige point(s)";
    }
    int length = player_s.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = player_s[i];
    }

    //bonus string
    vpos++;
    string bonus_s = "Discounts: ";
    length = bonus_s.length();
    for(int i = 0; i < GEMS-1; i++){
        int bonus = p->getDiscount(i);
        bonus_s += "|" + to_string(bonus) + "| ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
        if(bonus >= 10){
            length += 1;
        }
    }
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = bonus_s[i];
    }

    //gem string
    vpos++;
    string gems_s = "     Gems: ";
    length = gems_s.length();
    int different_gems = GEMS;
    for(int i = 0; i < different_gems; i++){
        int gems = p->getGemCount(i);
        gems_s += "(" + to_string(gems) + ") ";

        board_color[vpos][hpos + length] = indexToColor(i);
        board_color[vpos][hpos + length + 1] = indexToColor(i);
        board_color[vpos][hpos + length + 2] = indexToColor(i);

        length += 4;
    }
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = gems_s[i];
    }

    //reserve string
    vpos++;
    string reserve_s = " Reserved:";
    length = reserve_s.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = reserve_s[i];
    }

    hpos += reserve_s.length();
    for(int i = 0; i < 3; i++){
        Card *c = p->getReserve(i);
        if(c == nullptr){
            drawDeck(vpos, hpos, "");
        }else{
            drawCard(vpos, hpos, c);
        }
        hpos += CARD_WIDTH + 1;
    }
}

//draws a card deck at the specified position of the board
void Game::drawDeck(int vpos, int hpos, string type){
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];
        board[vpos + 1][hpos + i] = second[i];
        for(int j = 2; j < CARD_HEIGHT; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
    }

    int start_pos = hpos + (blank.length() / 2) - (type.length() / 2);
    length = type.length();
    for(int i = 0; i < length; i++){
        board[vpos + (CARD_HEIGHT/2)][start_pos + i] = type[i];
    }
}

//draws a gem at the specified position of the board
void Game::drawGem(int vpos, int hpos, string color){
    string gem_image[6];
    gem_image[0] = "  _______  ";
    gem_image[1] = ".'_/_|_\\_'.";
    gem_image[2] = "\\`\\  |  /`/";
    gem_image[3] = " `\\  | //' ";
    gem_image[4] = "   `\\|/`   ";
    gem_image[5] = "     `     ";

    for(int i = 0; i < GEMS; i++){
        int length = gem_image[i].length();
        for(int j = 0; j < length; j++){
            board[vpos + i][hpos + j] = gem_image[i][j];
            board_color[vpos + i][hpos + j] = color;
        }
    }
}

//draws a card at the specified position of the board
void Game::drawCard(int vpos, int hpos, Card *c){
    string top = "  _________  ";
    string second = " /         \\ ";
    string blank = "|           |";
    string bottom = " \\_________/ ";

    //draw the basic card outline
    int length = top.length();
    for(int i = 0; i < length; i++){
        board[vpos][hpos + i] = top[i];

        if(i == 8 || i == 9){
            board_color[vpos + 1][hpos + i] = c->getGemColor();
        }
        board[vpos + 1][hpos + i] = second[i];

        for(int j = 2; j < CARD_HEIGHT; j++){
            board[vpos + j][hpos + i] = blank[i];
        }
        board[vpos + CARD_HEIGHT][hpos + i] = bottom[i];
    }

    //draw the prestige number and gem
    board[vpos + 2][hpos + 3] = c->getPrestige() + 48;
    board_color[vpos + 2][hpos + 8] = c->getGemColor();
    board_color[vpos + 2][hpos + 9] = c->getGemColor();
    board[vpos + 2][hpos + 8] = '\\';
    board[vpos + 2][hpos + 9] = '/';
    board_color[vpos + 1][hpos + 8] = c->getGemColor();
    board_color[vpos + 1][hpos + 9] = c->getGemColor();
    board[vpos + 1][hpos + 8] = '_';
    board[vpos + 1][hpos + 9] = '_';

    //draw the prices
    vpos += 4;
    for(int i = 0; i < GEMS-1; i++){
        int price = c->getPrice(i);
        if(price > 0){
            board_color[vpos][hpos + 2] = indexToColor(i);
            board_color[vpos][hpos + 3] = indexToColor(i);
            board_color[vpos][hpos + 4] = indexToColor(i);
            board[vpos][hpos + 2] = '(';
            board[vpos][hpos + 3] = price + 48;
            board[vpos][hpos + 4] = ')';
            vpos++;
        }
    }

}

//clear the board display
void Game::clearBoard(){
    for(int i = 0; i < BOARD_HEIGHT; i++){
        for(int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
   }  
}