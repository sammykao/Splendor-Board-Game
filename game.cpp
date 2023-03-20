/*
 * game.cpp
 * CS11 Splendor
 * Edited by: Sammy Kao
 * Date: 12/8/22
 * Purpose: Implement a game class in splendor game.
 */

#include "game.h"
#include <string>

//constructor: Initialize all our values, create the decks for the 
// nobles, vendors, mines, transport, create 2 players
Game::Game(string filename, string play_nobles){
   //store gem colors for easy reference in drawing functions 
    colors[0] = "white";
    colors[1] = "red";
    colors[2] = "pink";
    colors[3] = "blue";
    colors[4] = "green";
    colors[5] = "gold";

   //include nobles or not
    if(play_nobles == "true") {
        this->play_nobles = true;
    } else {
        this->play_nobles = false;
    }

   //initialize the board
    for(int i = 0; i < BOARD_HEIGHT; i++) {
        for(int j = 0; j < BOARD_WIDTH; j++){
            board[i][j] = ' ';
            board_color[i][j] = "white";
        }
    }
    //create our decks
    mines = new Deck(filename, "m");
    transport = new Deck(filename, "t");
    vendors = new Deck(filename, "v");
    nobles = new Deck(filename, "n");

    //draw the cards and make gem bank
    for (int i=0; i < 4; i++) {
        Card *initcard = vendors->draw();
        grid[0][i] = initcard;
    }
    for (int i=0; i < 4; i++) {
        Card *initcard = transport->draw();
        grid[1][i] = initcard;
    }
    for (int i=0; i < 4; i++) {
        Card *initcard = mines->draw();
        grid[2][i] = initcard;
    }
    for (int i=0; i < 5; i++) {
        gem_bank[i] = 4;
    }
    gem_bank[5] = 5;
    // Create players
    players[0] = new Player();
    players[1] = new Player();

}

// Destructor: deallocate the grid, delete all decks and players
Game::~Game() {
    for (int i=0; i < 4; i++) {
        grid[0][i] = nullptr;
    }
    for (int i=0; i < 4; i++) {
        grid[1][i] = nullptr;
    }
    for (int i=0; i < 4; i++) {
        grid[2][i] = nullptr;
    }
    delete vendors;
    delete transport;
    delete mines;
    delete nobles;
    delete players[0];
    delete players[1];
    

}

//display()
//Input: none
//Description: Displays everything needed to be 
//displayed during player turn
//Output: returns nothing, but clears and prints board
void Game::display() {
    clearBoard();
    screen_clear();
    screen_home();
     drawBoard();
}

//playGame()
//Input: none
// Description: Main game loop, queries 
// and performs users move
//Output: Returns nothing, but prints board and prompts user
void Game::playGame(){
    bool game_over = false;
    int playerNum = 0;
    while(!game_over){
        bool playerTurn = false;
        display();
        string move;
        cout << "PLAYER " << (playerNum + 1) << ", enter your next move: ";
        while (!playerTurn) {
            cin >> move;
            if (move == "q") {
                return;
            } else {
                playerTurn = takeTurn(move, playerNum);
            }
        }
        if (play_nobles) {
            getNobles(players[playerNum]);
        }
        if (players[playerNum]->totalPrestige() >= 15) {
            if (playerNum == 0) {
                oneMoreTurn(playerNum + 1);
            } else {
                display();
                cout << "Congratulations Player 2: YOU WIN!!!" << endl;
            }
            game_over = true;
        }
        playerNum++;
        playerNum = playerNum % 2;
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
    if(!vendors->empty()){
        drawDeck(vpos, hpos, "Vendors");
    }
    vpos += CARD_HEIGHT + 1;
    if(!transport->empty()){
        drawDeck(vpos, hpos, "Transport");
    }
    vpos += CARD_HEIGHT + 1;
    if(!mines->empty()){
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
    drawPlayer(vpos + 1, 1, players[0]);
    drawPlayer(vpos + 1, ((CARD_WIDTH + 1) * 4), players[1]);

    //draw nobles
    if(play_nobles){
        hpos = ((CARD_WIDTH + 1) * 7) + 2;
        vpos = 2;
        for(int i = 0; i < NUM_NOBLES; i++){
            if(nobles->at(i) != nullptr){
                drawNoble(vpos, hpos, nobles->at(i));
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
// setGemBank()
// Input: color integer and amount
// Description: add or subtract values from the game gem bank
// Output: none
void Game::setGemBank(int color, int amount) {
    gem_bank[color] += amount;
}

//drawreset()
//Input: Two integers to vertical and horizontal
// position of card on grid
//Description: Draws and replaces the bought/reserved
// card on board grid
//Output:none, just draws and replaces card
void Game::drawreset(int vpos, int hpos) {
    if (vpos == 0) {
        Card *draw_card = vendors->draw();
        grid[vpos][hpos] = draw_card;
    } else if (vpos == 1) {
        Card *draw_card = transport->draw();
        grid[vpos][hpos] = draw_card;
    } else {
        Card *draw_card = mines->draw();
        grid[vpos][hpos] = draw_card;
    }
}
// buyDeckCard()
// Input: player pointer, integers of location of card
// Description: buy a card from board and add discounts,
// prestige, and return gems back to gem bank
// Output: none
bool Game::buyDeckCard(Player *player) {
    string vpos;
    int hpos;
    cin >> vpos >> hpos;
    if (typeChecker(vpos) == 3) {
        return false;
    }
    if (!columnChecker(hpos)) {
        return false;
    }
    Card *purchaseCard = grid[typeChecker(vpos)][hpos - 1];
    if (player->enoughGems(purchaseCard)) {
        int difference = 0;
        for(int i=0; i<5; i++) {
            int playerGems = player->getGemCount(i);
            int priceAmt = purchaseCard->getPrice(i);
            if (player->getDiscount(i) < priceAmt) {
                if (playerGems < priceAmt) {
                    setGemBank(i, playerGems);
                    if (!(player->getDiscount(i) >= (priceAmt - playerGems))) {
                        difference += (priceAmt - (playerGems + player->getDiscount(i)));
                    } 
                } else {
                    setGemBank(i, purchaseCard->getPrice(i));
                }
            }
        }
        setGemBank(5, difference);
        player->purchaseCard(purchaseCard);
        drawreset(typeChecker(vpos), hpos - 1);
        return true;
    }
    return false;
}

// buyReserveCard()
// Input: player pointer
// Description: recieve user input, check the input, 
// and buy a card from reserve and add discounts,
// prestige, and return gems back to gem bank if the input
// is valid
// Output: return true if action is performed, false if not
bool Game::buyReserveCard(Player *player) {
    int hpos;
    cin >> hpos;
    if (!columnChecker(hpos + 10)) {
        return false;
    }
    Card *purchaseCard = player->getReserve(hpos - 1);
    if (!player->enoughGems(purchaseCard)) {
        return false;
    }
    int difference = 0;
    for(int i=0; i<5; i++) {
        int playerGems = player->getGemCount(i);
        int priceAmt = purchaseCard->getPrice(i);
        if (player->getDiscount(i) < priceAmt) {
            if (playerGems < priceAmt) {
                setGemBank(i, playerGems);
                if (!(player->getDiscount(i) >= (priceAmt - playerGems))) {
                    difference += (priceAmt - (playerGems + player->getDiscount(i)));
                } 
            } else {
                setGemBank(i, purchaseCard->getPrice(i));
            }
        }
    }
    setGemBank(5, difference);
    player->purchaseCard(purchaseCard);
    player->emptyReserve(hpos - 1);
    return true;
}

// getDiffGems()
// Input: player pointer
// Description: recieve user input, check the input, and 
// add one of each color to the players gem count
// and remove one from gem bank if input is valid..
// Output: returns true if action is performed, false if not

bool Game::getDiffGems(Player *player) {
    string color[3];
    cin >> color[0] >> color[1] >> color[2];
    for (int i=0; i<3; i++) {
        if (!colorChecker(color[i])) {
            return false;
        }
        if (!GemChecker(color[i])) {
            return false;
        }
    }
    for (int i=0; i<3; i++) {
        if (color[i] == "white") {
            player->setGemCount(0, 1);
            setGemBank(0, -1);
        } else if (color[i] == "red") {
            player->setGemCount(1, 1);
            setGemBank(1, -1);
        } else if (color[i] == "pink") {
            player->setGemCount(2, 1);
            setGemBank(2, -1);
        } else if (color[i] == "blue") {
            player->setGemCount(3, 1);
            setGemBank(3, -1);
        } else if (color[i] == "green") {
            player->setGemCount(4, 1);
            setGemBank(4, -1);
        }
    }
    return true;
}

// getSameGems()
// Input: player pointer
// Description: recieve user input, check the input, and
// add two of the color to the players gem count if input is valid
// and remove two from gem bank
// Output: return true if action is performed, false if not

bool Game::getSameGems(Player *player) {
    string color;
    cin >> color;
    if (!colorChecker(color)) {
        return false;
    }
    if (!FullGemChecker(color)) {
        return false;
    }
    if (color == "white") {
        player->setGemCount(0, 2);
        setGemBank(0, -2);
    } else if (color == "red") {
        player->setGemCount(1, 2);
        setGemBank(1, -2);
    } else if (color == "pink") {
        player->setGemCount(2, 2);
        setGemBank(2, -2);
    } else if (color == "blue") {
        player->setGemCount(3, 2);
        setGemBank(3, -2);
    } else if (color == "green") {
        player->setGemCount(4, 2);
        setGemBank(4, -2);
    }
    return true;
}

// reserveCard()
// Input: player pointer
// Description: take in input, check the input, 
// and reserve a card to the player's reserve
// as long as he has space to reserve
// Output: return true if he reserves, false if not
bool Game::reserveCard(Player *player) {
    int hpos;
    string vpos;
    cin >> vpos >> hpos;
    if (typeChecker(vpos) == 3) {
        return false;
    }
    if (!columnChecker(hpos)) {
        return false;
    }
    Card *reserveCard = grid[typeChecker(vpos)][hpos - 1];
    bool cardreserved = false;
    for (int i=0; i<3; i++) {
        if (!cardreserved) {
            if (player->getReserve(i) == nullptr) {
                player->setReserve(i, reserveCard);
                cardreserved = true;
            }
        }
    }
    if (!cardreserved) {
        cout << "Invalid move (reserve is already full) - enter a valid move:";
        return false;
    }
    if (gem_bank[5] > 0) {
        setGemBank(5, -1);
        player->setGemCount(5, 1);
    }
    drawreset(typeChecker(vpos), hpos - 1);
    return true;
}

// inputChecker()
// Input: string of input
// Description: checks to see if a move is valid by
// the first string
// Output: return true if valid, false if not
bool Game::inputChecker(string move) {
    if (move == "q" || move == "p3" || move == "p2") {
        return true;
    } else if (move == "r" || move == "b" || move == "br") {
        return true;
    }
    cout << "Invalid move - enter a valid move: ";
    return false;
}

// colorChecker()
// Input: string of color
// Description: checks to see if a color is valid 
// Output: return true if valid, false if not
bool Game::colorChecker(string color) {
    if (color == "white" || color == "red") {
        return true;
    }
    if (color == "pink" || color == "blue") {
        return true;
    } 
    if (color == "green") {
        return true;
    }
    cout << "Invalid move (invalid color) - enter a valid move: ";
    return false;
}

// columnChecker()
// Input: integer of column
// Description: checks to see if a column is valid
// Output: return true if valid, false if not
bool Game::columnChecker(int hpos) { 
    if (hpos > 10) {
        if ((hpos - 10) > 0 && (hpos - 10) < 4) {
            return true;
        } else {
            cout << "Invalid move (invalid index) - enter a valid move: ";
            return false;
        }
    } 
    if (hpos < 5 && hpos > 0) {
        return true;
    }
    cout << "Invalid move (invalid column) - enter a valid move: ";
    return false;
}

// typeChecker()
// Input: string of type
// Description: checks to see if the user inputted
// type is valid
// Output: if it is valid, return vertical location, return 3 if not
int Game::typeChecker(string vpos) {
    if (vpos == "m") {
        return 2;
    }
    if (vpos == "t") {
        return 1;
    }
    if (vpos == "v") {
        return 0;
    }
    cout << "Invalid move (invalid card type) - enter a valid move: ";
    return 3;
}

// FullGemChecker()
// Input: string of color
// Description: checks to see if the color in gem
// bank is full for p2 move
// Output: return true if full, false if not
bool Game::FullGemChecker(string color) {
    if (color == "white") {
        if (gem_bank[0] >= 4) {
            return true;
        }
    }
    if (color == "red") {
        if (gem_bank[1] >= 4) {
            return true;
        }
    }
    if (color == "pink") {
        if (gem_bank[2] >= 4) {
            return true;
        }
    }
    if (color == "blue") {
        if (gem_bank[3] >= 4) {
            return true;
        }
    }
    if (color == "green") {
        if (gem_bank[4] >= 4) {
            return true;
        }
    }
    cout << "Invalid move (unavailable gems) - enter a valid move: ";
    return false;
}

// setOverGems()
// Input: player pointer and number of player
// Description: remove one gems if the player has 
// more than 10 gems
// Output: return true if able to removee a gem
bool Game::SetOverGems(Player *player, int playerNum) {
    clearBoard();
    screen_clear();
    screen_home();
    drawBoard();
    string move;
    cout << "PLAYER " << (playerNum + 1) << ", you have more than 10 gems!";
    cout << " Name one gem you would like to return: ";
    cin >> move;
    if (move == "white") {
        player->setGemCount(0, -1);
        gem_bank[0] += 1;
    } else if (move == "red") {
        player->setGemCount(1, -1);
        gem_bank[1] += 1;
    } else if (move == "pink") {
        player->setGemCount(2, -1);
        gem_bank[2] += 1;
    } else if (move == "blue") {
        player->setGemCount(3, -1);
        gem_bank[3] += 1;
    } else if (move == "green") {
        player->setGemCount(4, -1);
        gem_bank[4] += 1;
    } else if (move == "gold") {
        player->setGemCount(5, -1);
        gem_bank[5] += 1;
    } 
    if (colorChecker(move)) {
        return true;
    }
    return false;
 }

// GemChecker()
// Input: string of color
// Description: checks to see if gem pile of that color
// is empty
// Output: return true if not empty, false if so
bool Game::GemChecker(string color) {
    if (color == "white") {
        if (gem_bank[0] > 0) {
            return true;
        }
    }
    if (color == "red") {
        if (gem_bank[1] > 0) {
            return true;
        }
    }
    if (color == "pink") {
        if (gem_bank[2] > 0) {
            return true;
        }
    }
    if (color == "blue") {
        if (gem_bank[3] > 0) {
            return true;
        }
    }
    if (color == "green") {
        if (gem_bank[4] > 0) {
            return true;
        }
    }
    cout << "Invalid move (unavailable gems) - enter a valid move: ";
    return false;
}

//oneMoreTurn()
//input: integer of player number
// descri
void Game::oneMoreTurn(int playerNum) {
    display();
    string move;
    cout << "PLAYER 2, enter your next move: ";
    bool playerTurn = false;
    while (!playerTurn) {
        cin >> move;
        if (move == "q") {
                return;
        } else {
            playerTurn = takeTurn(move, playerNum);
        }
    }
    while(!players[playerNum]->PlayerGemChecker()) {
        while (!SetOverGems(players[playerNum], playerNum)) {
            continue;
        }
    }
    if (play_nobles) {
        getNobles(players[playerNum]);
    }
    display();
    if (players[1]->totalPrestige() > players[0]->totalPrestige()) {
        cout << "Congratulations Player 2: YOU WIN!!!" << endl;
    } else if (players[1]->totalPrestige() == players[0]->totalPrestige()) {
        if (lessPurchases() < 3) {
            cout << "Congratulations Player " << lessPurchases();
            cout << ": YOU WIN!!!" << endl;
        } else {
            cout << "You losers tied - BORING!!!" << endl;
        }
    } else {
        cout << "Congratulations Player 1: YOU WIN!!!" << endl;
    }
    return;
}
// takeTurn()
// Input: string of move and playerNum
// Description: execuetes a move based on player input
// Output: return true if move execueted, false if not

bool Game::takeTurn(string move, int playerNum) {
    if (!inputChecker(move)) {
        return false;
    }
    if (move == "p3") {
        if (!getDiffGems(players[playerNum])) {
            return false;
        }
    } else if (move == "p2") {
        if (!getSameGems(players[playerNum])) {
            return false;
        }
    } else if (move == "r") {
        if (!reserveCard(players[playerNum])) {
                return false;
        }
    } else if (move == "b") {
        if (!buyDeckCard(players[playerNum])) {
            return false;
        }
    } else if (move == "br"){
        if (!buyReserveCard(players[playerNum])) {
            return false;
        }
    } 
    while(!players[playerNum]->PlayerGemChecker()) {
        while (!SetOverGems(players[playerNum], playerNum)) {
            continue;
        }
    }
    return true;
}

/// getNobles() 
/// Input: pointer to player that gets checked on
/// Description: Checks to see if player qualifies
/// to get noble. If so, add points and shift noble
/// deck one down.
/// Output: none.
void Game::getNobles(Player *player) {
    for (int i=0; i<NUM_NOBLES; i++) {
        Card *noble = nobles->at(i);
        int difference = 0;
        for (int j=0; j<5; j++) {
            if (player->getDiscount(j) >= noble->getPrice(j)) {
                difference += 1;
            }
        }
        if (difference > 4) {
            player->setTotalPrestige(noble->getPrestige());
            nobles->setNobles(i);
        }
    }
    return;
}

//lessPurchases
//Input: none
//Description: See which player has purchased less cards
//Output: return player number of player with less purchases,
// 3 if tied.
int Game::lessPurchases() {
    int p1 = players[0]->getPurchases();
    int p2 = players[1]->getPurchases();
    if (p1 > p2) {
        return 2;
    } else if (p2 > p1) {
        return 1;
    } else {
        return 3;
    }
}